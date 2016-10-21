Simple ray tracing software with lights:

	To install : make

	To launch : ./raycaster width height input.json output.ppm

input.json format example:

[
  {"type": "camera",
    "width": 2.0,
    "height": 2.0
  },
  {"type": "sphere",
    "radius": 2.0,
    "diffuse_color": [1, 0, 0],
    "specular_color": [1, 1, 1],
    "position": [1, 1, 5]
  },
  {"type": "plane",
    "normal": [0, 1, 0],
    "diffuse_color": [0, 1, 0],
    "specular_color": [1, 1, 1],
    "position": [0, -1, 0]
  },
  {"type": "light",
    "color": [2, 2, 2],
    "theta": 0,
    "radial-a2": 0.125,
    "radial-a1": 0.125,
    "radial-a0": 0.125,
    "position": [1, 3, 1]
  }
]


The outpute will be an image in P6 ppm format.

RETURN value :	- 0 = normal
								- 1 = json parser Error
								- 2 = raycasting Error
								- 3 = Writting data Error
