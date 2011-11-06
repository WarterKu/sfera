/***************************************************************************
 *   Copyright (C) 1998-2010 by authors (see AUTHORS.txt)                  *
 *                                                                         *
 *   This file is part of Sfera.                                           *
 *                                                                         *
 *   Sfera is free software; you can redistribute it and/or modify         *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Sfera is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "sdl/editaction.h"


#include "renderer/cpu/singlecpurenderer.h"
#include "acceleretor/bvhaccel.h"

SingleCPURenderer::SingleCPURenderer(const GameLevel *level) :
	LevelRenderer(level), rnd(1), timeSinceLastCameraEdit(WallClockTime()),
		timeSinceLastNoCameraEdit(WallClockTime()) {
	const unsigned int width = gameLevel->gameConfig->GetScreenWidth();
	const unsigned int height = gameLevel->gameConfig->GetScreenHeight();

	passFrameBuffer = new FrameBuffer(width, height);
	filterFrameBuffer = new FrameBuffer(width, height);
	frameBuffer = new FrameBuffer(width, height);
	toneMapFrameBuffer = new FrameBuffer(width, height);

	passFrameBuffer->Clear();
	filterFrameBuffer->Clear();
	frameBuffer->Clear();
	toneMapFrameBuffer->Clear();
}

SingleCPURenderer::~SingleCPURenderer() {
	delete passFrameBuffer;
	delete filterFrameBuffer;
	delete frameBuffer;
	delete toneMapFrameBuffer;
}

Spectrum SingleCPURenderer::SampleImage(const Accelerator &accel,
		const float u0, const float u1) {
	Ray ray;
	gameLevel->camera->GenerateRay(
		u0, u1,
		passFrameBuffer->GetWidth(), passFrameBuffer->GetHeight(),
		&ray, rnd.floatValue(), rnd.floatValue());

	const Scene &scene(*(gameLevel->scene));
	Spectrum throughput(1.f, 1.f, 1.f);
	Spectrum radiance(0.f, 0.f, 0.f);

	unsigned int diffuseBounces = 0;
	const unsigned int maxDiffuseBounces = gameLevel->maxPathDiffuseBounces;
	unsigned int specularGlossyBounces = 0;
	const unsigned int maxSpecularGlossyBounces = gameLevel->maxPathSpecularGlossyBounces;

	const vector<GameSphere> &spheres(scene.spheres);
	for(;;) {
		// Check for intersection with objects
		unsigned int sphereIndex;
		if (accel.Intersect(&ray, &sphereIndex)) {
			const Sphere *hitSphere;
			const Material *hitMat;
			const TexMapInstance *texMap;
			const BumpMapInstance *bumpMap;

			if (sphereIndex >= spheres.size()) {
				// I'm hitting the puppet
				const unsigned int puppetIndex = sphereIndex - spheres.size();
				
				hitSphere = &(gameLevel->player->puppet[puppetIndex]);
				hitMat = gameLevel->player->puppetMaterial[puppetIndex];
				texMap = NULL;
				bumpMap = NULL;
			} else {
				hitSphere = &spheres[sphereIndex].sphere;
				hitMat = scene.sphereMaterials[sphereIndex];
				texMap = scene.sphereTexMaps[sphereIndex];
				bumpMap = scene.sphereBumpMaps[sphereIndex];
			}

			const Point hitPoint(ray(ray.maxt));
			Normal N(Normalize(hitPoint - hitSphere->center));

			// Check if I have to flip the normal
			Normal ShadeN = (Dot(Vector(N), ray.d) > 0.f) ? (-N) : N;
			if (bumpMap) {
				// Apply bump mapping
				const Vector dir = Normalize(hitPoint - hitSphere->center);

				ShadeN = bumpMap->SphericalMap(dir, ShadeN);
			}

			radiance += throughput * hitMat->GetEmission();

			Vector wi;
			float pdf;
			bool diffuseBounce;
			Spectrum f = hitMat->Sample_f(-ray.d, &wi, N, ShadeN,
					rnd.floatValue(), rnd.floatValue(), rnd.floatValue(),
					&pdf, diffuseBounce);
			if ((pdf <= 0.f) || f.Black())
				return radiance;

			// Check if there is a texture map to apply
			if (texMap) {
				const Vector dir = Normalize(hitPoint - hitSphere->center);
				f *= texMap->SphericalMap(dir);
			}

			if (diffuseBounce) {
				++diffuseBounces;

				if (diffuseBounces > maxDiffuseBounces)
					return radiance;
			} else {
				++specularGlossyBounces;

				if (specularGlossyBounces > maxSpecularGlossyBounces)
					return radiance;
			}

			throughput *= f / pdf;

			ray = Ray(hitPoint, wi);
		} else
			return radiance + throughput * scene.infiniteLight->Le(ray.d);
	}
}

void SingleCPURenderer::ApplyBoxFilterX(const Pixel *src, Pixel *dest,
	const unsigned int width, const unsigned int height, const unsigned int radius) {
    const float scale = 1.0f / (float)((radius << 1) + 1);

    // Do left edge
    Pixel t = src[0] * radius;
    for (unsigned int x = 0; x < (radius + 1); ++x)
        t += src[x];
    dest[0] = t * scale;

    for (unsigned int x = 1; x < (radius + 1); ++x) {
        t += src[x + radius];
        t -= src[0];
        dest[x] = t * scale;
    }

    // Main loop
    for (unsigned int x = (radius + 1); x < width - radius; ++x) {
        t += src[x + radius];
        t -= src[x - radius - 1];
        dest[x] = t * scale;
    }

    // Do right edge
    for (unsigned int x = width - radius; x < width; ++x) {
        t += src[width - 1];
        t -= src[x - radius - 1];
        dest[x] = t * scale;
    }
}

void SingleCPURenderer::ApplyBoxFilterY(const Pixel *src, Pixel *dst,
	const unsigned int width, const unsigned int height, const unsigned int radius) {
    const float scale = 1.0f / (float)((radius << 1) + 1);

    // Do left edge
    Pixel t = src[0] * radius;
    for (unsigned int y = 0; y < (radius + 1); ++y) {
        t += src[y * width];
    }
    dst[0] = t * scale;

    for (unsigned int y = 1; y < (radius + 1); ++y) {
        t += src[(y + radius) * width];
        t -= src[0];
        dst[y * width] = t * scale;
    }

    // Main loop
    for (unsigned int y = (radius + 1); y < (height - radius); ++y) {
        t += src[(y + radius) * width];
        t -= src[((y - radius) * width) - width];
        dst[y * width] = t * scale;
    }

    // Do right edge
    for (unsigned int y = height - radius; y < height; ++y) {
        t += src[(height - 1) * width];
        t -= src[((y - radius) * width) - width];
        dst[y * width] = t * scale;
    }
}

void SingleCPURenderer::ApplyBoxFilter(Pixel *frameBuffer, Pixel *tmpFrameBuffer,
	const unsigned int width, const unsigned int height, const unsigned int radius) {
	for (unsigned int i = 0; i < height; ++i)
		ApplyBoxFilterX(&frameBuffer[i * width], &tmpFrameBuffer[i * width], width, height, radius);

	for (unsigned int i = 0; i < width; ++i)
		ApplyBoxFilterY(&tmpFrameBuffer[i], &frameBuffer[i], width, height, radius);
}

void SingleCPURenderer::DrawFrame(const EditActionList &editActionList) {
	//--------------------------------------------------------------------------
	// Build the Accelerator
	//--------------------------------------------------------------------------

	const int treeType = 4; // Tree type to generate (2 = binary, 4 = quad, 8 = octree)
	const int isectCost = 80;
	const int travCost = 10;
	const float emptyBonus = 0.5f;

	const vector<GameSphere> &spheres(gameLevel->scene->spheres);
	vector<const Sphere *> sphereList(gameLevel->scene->spheres.size() + GAMEPLAYER_PUPPET_SIZE);
	for (size_t s = 0; s < spheres.size(); ++s)
		sphereList[s] = &(spheres[s].sphere);
	for (size_t s = 0; s < GAMEPLAYER_PUPPET_SIZE; ++s) {
		const Sphere *puppet = &(gameLevel->player->puppet[s]);

		sphereList[s + spheres.size()] = puppet;
	}

	BVHAccel accel(sphereList, treeType, isectCost, travCost, emptyBonus);

	//--------------------------------------------------------------------------
	// Render
	//--------------------------------------------------------------------------

	const GameConfig &gameConfig(*(gameLevel->gameConfig));
	const unsigned int width = gameConfig.GetScreenWidth();
	const unsigned int height = gameConfig.GetScreenHeight();
	const unsigned int samplePerPass = gameConfig.GetSingleCPUSamplePerPass();

	// Render the frame
	const float sampleScale = 1.f / samplePerPass;
	for (unsigned int i = 0; i < samplePerPass; ++i) {
		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				Spectrum s = SampleImage(accel,
						x + rnd.floatValue() - .5f, y + rnd.floatValue() - .5f) *
						sampleScale;

				if (i == 0)
					passFrameBuffer->SetPixel(x, y, s);
				else
					passFrameBuffer->AddPixel(x, y, s);
			}
		}
	}

	//--------------------------------------------------------------------------
	// Apply a gaussian filter: approximated by applying a box filter multiple times
	//--------------------------------------------------------------------------

	const unsigned int filterPassCount = 3;
	for (unsigned int i = 0; i < filterPassCount; ++i)
		ApplyBoxFilter(passFrameBuffer->GetPixels(), filterFrameBuffer->GetPixels(), width, height, 1);

	//--------------------------------------------------------------------------
	// Blend the new frame with the old one
	//--------------------------------------------------------------------------

	float k;
	if (editActionList.Has(CAMERA_EDIT)) {
		timeSinceLastCameraEdit = WallClockTime();

		const double dt = Min(WallClockTime() - timeSinceLastNoCameraEdit, 2.0);
		k = 1.f - dt / 2.f;
	} else {
		timeSinceLastNoCameraEdit = WallClockTime();

		const double dt = Min(WallClockTime() - timeSinceLastCameraEdit, 5.0);
		k = dt / 5.f;
	}

	const float blendFactor = (1.f - k) * gameConfig.GetSingleCPUGhostFactorCameraEdit() +
		k * gameConfig.GetSingleCPUGhostFactorNoCameraEdit();

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			const Pixel *p = passFrameBuffer->GetPixel(x, y);
			frameBuffer->BlendPixel(x, y, *p, blendFactor);
		}
	}

	//--------------------------------------------------------------------------
	// Tone mapping
	//--------------------------------------------------------------------------

	gameLevel->toneMap->Map(frameBuffer, toneMapFrameBuffer);

	glDrawPixels(width, height, GL_RGB, GL_FLOAT, toneMapFrameBuffer->GetPixels());
}
