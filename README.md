# Bebo
![thumbnail_IMG_5279](https://github.com/user-attachments/assets/9f43f72a-443b-47fd-9350-2385fab826dc)

Bebo is an adorable shoulder-sitting friendly robot I built to bring to Open Sauce 2024! He runs on an ESP32-S3 cycling through several gifs to make various expressions.

## Early Concept
The basic requirements for the project included a fairly long battery life (a few hours), compact electronics, lightweight construction, and stable mounting to the shoulder to ensure it wouldn't faill off while wearing it.
![thumbnail_IMG_5015](https://github.com/user-attachments/assets/fde0c8d6-16b1-43cc-97a5-c1a5219825b2)

## CAD/Design
I started designing Bebo a week before the convention with the help of friends. On a tight time constrait, I opted for a very simple design that could be quickly 3d printed and assembled. 

I settled on a four piece design, with a removable (non-glued) top for easy access to the electronics for battery changes. The top fits snugly so it doesn't fall or loosen.

![thumbnail_Illustration3](https://github.com/user-attachments/assets/2830f659-b1fe-4959-bb75-58f7c63078f5)

![image](https://github.com/ryan123rudder/Bebo/assets/42309874/7f2e542c-a53c-44f5-a97a-ce439be06ce1)

![image](https://github.com/ryan123rudder/Bebo/assets/42309874/0448a3e5-8894-4c47-8e7d-8a48bfcc1bb0)

## 3D Printing
Bebo was printed on a Prusa Mk3 in PLA.
![thumbnail_IMG_5167](https://github.com/user-attachments/assets/42906f11-d742-41ff-9a9c-4ac08c4c1bd7)

## Mounting
Intiially, Bebo was mounted using weak magnents glued to his base which connected to a magnet pad glued to the undershirt I wore. These weren't strong enough, soI upgraded to neodymium magnets, which improved stability but still allowed for some tipping forward and backwards. On the day of the convention, I added KT tape to secure the magnent pad on my shoulder more securely.

![thumbnail_IMG_5173](https://github.com/user-attachments/assets/cd3a743f-5575-44ee-8e6f-9aade0827a20)

## Electronics
Bebo utilizes an ESP32-S3 connected to a 3.5" TFT bar display from Adafruit. All GIFs are loaded onto a microSD card. The power circuit consists of a battery pack and a voltage step-down module, reducing the 6V battery pack to 3.3V for the ESP32.

## Programming
The code for displaying GIFs gave me a lot of trouble, primarily with screen timings and poor refresh rates. The final version still has a low refresh rate, which is a major area for improvment in Bebo V2. The code for this project is contained in the repositiory.

## Field Testing
I had a great time walking around with Bebo at Open Sauce! I got to meet amazing people, including Jorvon Moss, a major inspiriation for the project. Overall, Bebo handled field testing really well. Towards the end of each day, the KT tape adhesive weaked, but this only resulted in slight tipping. Bebo did fall 3 times, with some minor repairable damage on the 3rd fall. He held up very well overall, was comfortable to wear for many hours, and was easy to remove and hand to people for a closer look.
![thumbnail_IMG_5377](https://github.com/user-attachments/assets/a4b5a7f8-5917-4d6e-a02f-af5548376fc4)

## Summary
Overall, this fun project taught me a lot about design, field robotics, and embedded coding. If I were to build Bebo again, I would use a smaller display, as the ribbon cable was damaged due to the sharp bend.
