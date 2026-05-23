# RG project 2025-26

`43/2021 - Stefan Novaković`

This project was created as part of a Computer Graphics course, at Faculty of Mathematics, University of Belgrade. It
presents a scene which contains a 3D model of the Kadinjača Memorial Complex.

The model is displayed using both point lighting and directional lighting. The point light provides illumination from a
specific position in the scene, while the directional light simulates sunlight (or moonlight, depending on the state
created by user events), creating consistent shading across the model.

In addition to its graphical aspects, the project also reflects the historical importance of Kadinjača, located 14km
from Užice, western Serbia. The memorial complex commemorates the workers’ battalion that defended the Užice Republic
during World War II. In November 1941, they made a final stand against advancing German forces, sacrificing their lives
to delay the enemy and protect the retreat of partisan units. Today, Kadinjača stands as a symbol of resistance and
remembrance. From Slavko Vukosavljević’s song 'Kadinjača':

```txt
[...]
Рођена земљо, јеси ли знала?
ту је погино батаљон цео...
Црвена крв је процветала
кроз снежни покров, хладан и бео.

Ноћу је и то завејо ветар.
ипак на југу... војска корача...
Пао је четрнаести километар,
али никад неће
Кадињача.
```

## Controls

- `w` - Move forward
- `s` - Move backward
- `a` - Move left
- `d` - Move right
- `arrow up` - Look up
- `arrow down` - Look down
- `arrow left` - Look left
- `arrow right` - Look right
- `shift` - Increase movement/rotation speed
- `F1` - Toggle camera info
- `F2` - Toggle help
- `F3` - Toggle point light menu
- `F4` - Toggle cursor
- `F5` - Trigger event
- `ESC` - Exit

## Features

### Fundamental

- [x] Model with lighting
- [x] Directional and point light, point light can be customized via GUI
- [x] **ACTION:** F5 --> **EVENT:** Slowly change from day to night and vice versa, automatically (half way through the
      transition) turning on/off the point lights to simulate the street lamps turning on/off

## Models

- [Kadinjaca](https://funes.world/entities/kadinjaca-memorial-complex/models/wy510h7rol2jr061b0rykxxv)

## Skyboxes

- [skybox](https://freestylized.com/skybox/sky_93/)

## External libraries (other than the ones provided by the project skeleton)

- [miniaudio](https://github.com/mackron/miniaudio)
