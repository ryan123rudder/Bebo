# Bebo
Bebo is an adorable shoulder-sitting friendly robot who I built to bring to Open Sauce 2024! He runs on an ESP32 S3 cycling through several gifs.

## Early Concept
The basic requirements for the project was a fairly long battery life (a few hours), compact electronics, light weight, and stable on the shoulder so I could wear it without worrying about it falling off.

## CAD/Design
I started designing Bebo a week before the convention. On a tight time constrait, i decided on a very simple design that could be quickly 3d printed and assembled. 

I seteled on this 4 piece design, with a removable (and not glued) top for easy access to the electronics for battery changes.

![image](https://github.com/ryan123rudder/Bebo/assets/42309874/7f2e542c-a53c-44f5-a97a-ce439be06ce1)

![image](https://github.com/ryan123rudder/Bebo/assets/42309874/0448a3e5-8894-4c47-8e7d-8a48bfcc1bb0)

## Mounting
Intiially, Bebo was mounted with some weak magnents glued to the bottom of him and to the undershirt I would be wearing. When these weren't strong enough I upgraded to neodymieum magnents, but he stilled tipped forward and backwards. Finally, on the day of the convention, I added KT tape to fix the magnent pad on my shoulder more securely.

## Electronics
Bebo utilizes an ESP32 wired to a 3.5" TFT Bar display from Adafruit. All of the gifs are loaded onto a micro SD card. The battery pack and voltage step down module (which takes the 6V battery pack down to 3.3V) make up the power circuit.

## Programming
The code for displaying GIFs gave me a lot of trouble, primarily with bad screen timings and poor refresh rates. The final version still has a low refresh rate, which is a major area for improvment for V2. The code for this project is contained in the repositiory.

## Field Testing
I had a great time walking around with Bebo at Open Sauce! I got to meet amazing people, including Jorvon Moss, a major inspiriation for the project. Overall, Bebo handled field testing really well. Towards the end of each day, the KT tape wouldn't hold as well, but this only really resulted in slight tipping. Bebo did fall 3 times, with some minor repairable damage on the 3rd fall. He held up very well overall, and was comfortable to wear for many hours, as well as being easy to take off and hand to people to look at.

## Summary
Overall, this fun project taught me a lot about design, field robotics, and embedded coding. 
