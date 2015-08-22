<table cellpadding='2' cellspacing='0'>
<tr><td valign='top'><img src='https://sfera.googlecode.com/hg/web/images/home-side.jpg' /></td>
<td valign='top'>
<h2>Sfera</h2>
Sfera is a game, mostly written using <a href='http://www.khronos.org'>OpenCL</a> and <a href='http://www.khronos.org'>OpenGL</a>, with real-time path tracing (i.e. ray tracing) rendering. It includes a <a href='http://www.bulletphysics.org'>Bullet Physics</a> based engine.<br>
<br>
<h2>Video</h2>

A Sfera demo video is available <a href='http://www.youtube.com/watch?v=Dh9uWYaiP3s'>here</a>.<br>
<br>
<h2>News</h2>

Check the <a href='News.md'>news wiki page</a> for any Sfera related news.<br>
<br>
<h3>Features</h3>
Sfera supports:<br>
<ul>
<li>Multiple render engines: mono-thread, multi-thread, mono-GPU, multi-GPUs;</li>
<li>Multiple materials (all can emit light): matte, mirror, glass, metal, alloy;</li>
<li>Texture mapping;</li>
<li>Bump mapping;</li>
<li>Depth of field;</li>
<li>A text SDL (Scene Description Language) for levels;</li>
<li>Multi-platforms (i.e. Linux, MacOS, Windows, etc.);</li>
<li>A single awesome primitive supported: sphere;</li>
</ul>
<p />

<h3>Authors</h3>
<ul>
<li>David "Dade" Bucciarelli <dade916.at.gmail.com></li>
</ul>
Sfera is heavly based on <a href='http://www.luxrender.net/wiki/SLG'>SmallLuxGPU v2.0</a> and many other classes from <a href='http://www.luxrender.net/en_GB/index'>LuxRender</a> project.<br>
<br>
<h3>Credits</h3>
A special thanks goes to:<br>
<ul>
<li> <a href='http://www.bulletphysics.org'>http://www.bulletphysics.org</a> for the physic engine;</li>
<li> <a href='http://freeimage.sourceforge.net'>http://freeimage.sourceforge.net</a> for open source image library;</li>
<li> <a href='http://www.libsdl.org'>http://www.libsdl.org</a> for cross-platform multimedia library;</li>
<li> <a href='http://www.hdrlabs.com/sibl/archive.html'>http://www.hdrlabs.com/sibl/archive.html</a> for HDR maps;</li>
<li> <a href='http://freebitmaps.blogspot.com'>http://freebitmaps.blogspot.com</a> for planet texture maps;</li>
</ul>

This software is released under GPL v3 License (see <a href='http://code.google.com/p/sfera/source/browse/COPYING.txt'>COPYING.txt</a> file).<br>
<br>
<h3>History of ray tracing and World of Spheres</h3>

I trace the root of world of spheres up to the wonderful Kevin Beason's <a href='http://kevinbeason.com/smallpt'>SmallPT</a>. Thierry Berger-Perrin ported SmallPT to CUDA with <a href='http://code.google.com/p/tokaspt'>Tokaspt</a>.<br>
I wrote <a href='http://davibu.interfree.it/opencl/smallptgpu/smallptGPU.html'>SmallPT GPU</a> and <a href='http://davibu.interfree.it/opencl/smallptgpu2/smallptGPU2.html'>SmallPT GPU2</a>: an OpenCL version of SmallPT, as soon as the ATI OpenCL SDK was released.<br>
<a href='http://raytracey.blogspot.com'>Ray Tracey</a>'s <a href='http://code.google.com/p/tokap-the-once-known-as-pong'>Tokap</a> has shown that real-time ray tracing is a viable option for games and world of spheres.<br>
I hope Sfera will be another enjoyable chapter in the path of world of spheres !<br>
<br>
<br>
</td></tr>
</table>