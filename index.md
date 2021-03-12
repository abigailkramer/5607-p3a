## Project 3A

**Team Members:**
- Bayan Tawakalna (tawak005)
- Abigail Kramer (krame669)


**Additional Features (15pts):**
- Shadows
- Multiple Light Sources
- Directional Light Sources


**Extra Features (10pts):**
- Spot Light Sources
- Depth of Field
- Reflection

**Art Contest (1pt)**

**Brief Overview**

We based the structure of our program on the pseudocode presented in lecture. This helped us to organize each computation, along with making everything easy to follow. In PGA_3D.h we created structs for Spheres, HitInformation, and each light type. Each of which contained information that was important to a calculation involving that object. We were then able to use <vector> to efficiently keep lists of each item added from the text file. In image_lib.h we added operators for the Color struct, so we could efficiently calculate additions and multiplications for colors without going through each channel for each computation. We encountered some trouble when implementing shadows, which ended up being a lack of offset for the hit point.
  
  Since we stored our structs in lists, achieving the multiple light sources came fairly easy. Implementing the light sources (directional and spot), were simple to figure out once point lights had been established. Our extra features are the spot light sources and depth of field. We included reflection to showcase the depth of field, since the reflection itself isn't quite accurate.

---

Lights **not all from the same direction*

**Directional**

![directional_sphere](https://user-images.githubusercontent.com/59031606/110731867-b4c2e780-81e8-11eb-806d-f43d1b63ffbc.png)

**Point**

![point_sphere](https://user-images.githubusercontent.com/59031606/110860730-c2c54680-8282-11eb-952e-9ded0f1e1ae5.png)

**Spot**

![spot_sphere](https://user-images.githubusercontent.com/59031606/110731959-dde37800-81e8-11eb-8965-b812c40c72d4.png)


**Shadows, Color & Specularity, Multiple Lights**

![shadows_and_light](https://user-images.githubusercontent.com/59031606/110732132-2f8c0280-81e9-11eb-8ef9-94bb92a88cad.png)

**Reflection and Depth of Field**

![reflection](https://user-images.githubusercontent.com/80165299/110856395-3401fb00-827d-11eb-965a-8d42f7fa8b71.png)

**Ambient Light**

![center-sphere](https://user-images.githubusercontent.com/80165299/110858362-9cea7280-827f-11eb-85bd-097a63962888.png)

**Extra Test Image - overhead white light, and centered purple(ish) light**

![test](https://user-images.githubusercontent.com/59031606/110863866-ee4a3000-8286-11eb-99b5-51cebd66d614.png)

**Artistic Image**

![Artistic Image](https://user-images.githubusercontent.com/80165299/110858760-14200680-8280-11eb-920b-d0e6fcd5f3c3.png)
