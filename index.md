## Project 3A

**Team Members:**
- Bayan Tawakalna (tawak005)
- Abigail Kramer (krame669)


**Additional Features:**
- Shadows
- Multiple Light Sources
- Directional Light Sources


**Extra Features Completed:**
- Depth of Field
- Reflection
- *Spot Light Sources - presented, but we chose the other two for our extra credit features*

**Art Contest (1pt)**

**Brief Overview**

We based the structure of our program on the pseudocode presented in lecture. This helped us to organize each computation, along with making everything easy to follow. In PGA_3D.h we created structs for Spheres, HitInformation, and each light type. Each of which contained information that was important to a calculation involving that object. We were then able to use <vector> to efficiently keep lists of each item added from the text file. In image_lib.h we added operators for the Color struct, so we could efficiently calculate additions and multiplications for colors without going through each channel for each computation. We encountered some trouble when implementing shadows, which ended up being a lack of offset for the hit point.
  
  Since we stored our structs in lists, achieving the multiple light sources came fairly easy. Implementing the light sources (directional and spot), were simple to figure out once point lights had been established. Out extra extra credit features are depth of field and reflection. The reflection gave us a good deal of trouble, one road bump being the reflected line, the other being an issue with the updated hit point upon recursion.

---

Lights **not all from the same direction*

**Directional**

![directional_sphere](https://user-images.githubusercontent.com/59031606/110731867-b4c2e780-81e8-11eb-806d-f43d1b63ffbc.png)

**Point**

![point_sphere](https://user-images.githubusercontent.com/59031606/110860730-c2c54680-8282-11eb-952e-9ded0f1e1ae5.png)

**Spot**

![spot_sphere](https://user-images.githubusercontent.com/59031606/110731959-dde37800-81e8-11eb-8965-b812c40c72d4.png)


**Shadows, Color & Specularity, Multiple Lights**

![spheres2](https://user-images.githubusercontent.com/59031606/110883913-ce783380-82a9-11eb-9731-91a2e6e2d55f.png)

**Reflection and Depth of Field**

max_depth = 2

![depth2](https://user-images.githubusercontent.com/59031606/110887973-dedfdc80-82b0-11eb-9c43-369f444b86b1.png)

max_depth = 5

![depth5](https://user-images.githubusercontent.com/59031606/110887908-c7085880-82b0-11eb-85ea-f1c9569b8149.png)


**Ambient Light**

![center-sphere](https://user-images.githubusercontent.com/80165299/110858362-9cea7280-827f-11eb-85bd-097a63962888.png)

**Extra Test Image - overhead white light, and centered purple(ish) light**

![test_img](https://user-images.githubusercontent.com/59031606/110888146-367e4800-82b1-11eb-8624-4520b75b58f1.png)

**Artistic Image**

![Artistic Image](https://user-images.githubusercontent.com/80165299/110858760-14200680-8280-11eb-920b-d0e6fcd5f3c3.png)



