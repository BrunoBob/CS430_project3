Simple ray casting software :

	To install : make

	To launch : ./raycaster width height input.json output.ppm

input.json format example:

[
 {
    "type": "camera",
    "width": 1,
    "height": 1
 },
 {
    "type": "sphere",
    "color": [1, 0, 0],
    "position": [1, 1, 10],
    "radius": 2
 },
 {
    "type": "plane",
    "color": [0, 1, 0],
    "position": [0, -1, 0],
    "normal": [0, 1, 0]
 }
]

The outpute will be an image in P6 ppm format.

RETURN value :	- 0 = normal
								- 1 = json parser Error
								- 2 = raycasting Error
								- 3 = Writting data Error
