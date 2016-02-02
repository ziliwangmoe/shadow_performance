## Build suggestion
Please use qmake to get the project files you like, I use visual studio 2013

## Content
* 1, I used framebuffer object to get the depth of a scene.
* 2, Use the depth of a the scene as texture, I rendered the shadow.
* 3, Experiment with impacts of branching, int vs. float, inline vs. function call.

    I used very simple fragment shader. In the shader, if the statement is true, it will enter of great amount of iteration, otherwise it will almost do nothing. In the iteration, I can test the impact of data type and function call. And also by changing the statement, I can study the pattern of influence of branching.

## Instruction
Using UI, choose the shader to view:
* Scene with shadow
* Depth of the scene
* Performance test

    In performance test, you can adjust the count of iteration, and the size of computational intensive area. So, you can get different level of computational burden. Also, you can choose whether use int or float, inline code or function call to see the difference in term of FPS

## Conclusion

The impact of branching is related to localization. I find that if I keep the height of intensive computational area minimum and change the width of the area, the FPS doesn’t change very much.  if I keep the width of intensive computational area minimum and change the height of the area, the FPS changes a lot. This maybe caused by that the synchronization of execution only happens in a warp or in a block. Maybe all pixels in a row are likely to be in a block. So, since there is just one pixel need to go through the iteration, all other pixels have to wait it.

Also, I find that using float is slightly more efficient than using int. This difference is more obvious when size of intensive computational area is large and iteration is less, compare to the opposite configuration in the same FPS.

Finally, I find there is no difference between using inline code or function call.
