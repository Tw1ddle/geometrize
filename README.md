[![Geometrize Logo](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/logo.png?raw=true "Geometrize logo")](https://www.geometrize.co.uk/)

[![Travis Build Status](https://img.shields.io/travis/Tw1ddle/geometrize.svg?style=flat-square)](https://travis-ci.org/Tw1ddle/geometrize)
[![Geometrize AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/09l5nquksmev8ta4?svg=true)](https://ci.appveyor.com/project/Tw1ddle/geometrize)
[![License](https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=flat-square)](https://github.com/Tw1ddle/geometrize/blob/master/LICENSE)

[Geometrize](https://www.geometrize.co.uk/) is a desktop app that geometrizes images into geometric primitives.

Download Geometrize [here](https://www.geometrize.co.uk/), or run the [web demo](https://www.samcodes.co.uk/project/geometrize-haxe-web/) in your browser.

[![Geometrize Shape Animation](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/windflowers_geometrized.gif?raw=true)](https://www.geometrize.co.uk/)

## Features

 * Recreate images as geometric primitives.
 * Start with hundreds of images with preset settings.
 * Export geometrized images to SVG, PNG, JPG, GIF and more.
 * Export geometrized images as HTML5 canvas or WebGL webpages.
 * Export shape data as JSON for use in custom projects and creations.
 * Control the algorithm at the core of Geometrize with ChaiScript scripts.

## Usage

Open the app, select an image, pick your desired shape types, and hit start. Video tutorials for Geometrize are posted [here](https://www.youtube.com/playlist?list=PLe9ogi_J4cFgcqLdpmPC7GdFV5ohJPEzN).

[![Geometrize Getting Started Animation](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/startup_geometrized.gif?raw=true)](https://www.geometrize.co.uk/)

## Shape Comparison

The matrix shows results for circles, triangles, rotated rectangles, rotated ellipses and all supported shapes at 50, 200 and 500 total shapes:

| -                  | 50 Shapes     | 200 Shapes    | 500 Shapes   |
| ------------------ | ------------- | ------------- | ------------ |
| Circles            | [![50 Circles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_50_circles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Circles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_200_circles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Circles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_500_circles.png?raw=true)](https://www.geometrize.co.uk/) |
| Triangles          | [![50 Triangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_50_triangles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Triangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_200_triangles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Triangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_500_triangles.png?raw=true)](https://www.geometrize.co.uk/) |
| Rotated Rectangles | [![50 Rotated Rectangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_50_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Rotated Rectangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_200_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Rotated Rectangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_500_rotated_rectangles.png?raw=true)](https://www.geometrize.co.uk/) |
| Rotated Ellipses   | [![50 Rotated Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_50_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) | [![200 Rotated Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_200_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) | [![500 Rotated Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_500_rotated_ellipses.png?raw=true)](https://www.geometrize.co.uk/) |
| All Shapes         | [![50 All Shapes](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_50_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) | [![200 All Shapes](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_200_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) | [![500 All Shapes](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/seagull_500_all_shapes.png?raw=true)](https://www.geometrize.co.uk/) |

## Screenshots

For more examples of geometrized images, see the [gallery](https://gallery.geometrize.co.uk/).

[![Geometrized Flower 330 Rotated Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/flower.png?raw=true "Flower - 330 Rotated Ellipses")](https://www.geometrize.co.uk/)

[![Geometrized Train 230 Rotated Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/train.png?raw=true "Train - 230 Rotated Ellipses")](https://www.geometrize.co.uk/)

[![Geometrized Trees 210 Ellipses](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/tree_under_clouds.png?raw=true "Tree Under Clouds - 210 Ellipses")](https://www.geometrize.co.uk/)

[![Geometrized Woodland Cemetery 600 Rotated Rectangles](https://github.com/Tw1ddle/geometrize/blob/master/screenshots/woodland_cemetery.png?raw=true "Woodland Cemetery - 600 Rotated Rectangles")](https://www.geometrize.co.uk/)

## Resources

 * See the Geometrize [resources](https://resources.geometrize.co.uk/) page.

## Building

Geometrize is a Qt app written in C++.

 * Download and install Qt 5.10 or above.
 * Install Python 3.x and add it to your path. The build process includes a few Python scripts for creating resource files.
 * Checkout this repository and all submodules, and build and run [geometrize.pro](https://github.com/Tw1ddle/geometrize/blob/master/geometrize.pro) within Qt Creator.

## Notes
 * Got an idea or suggestion? Open an issue on GitHub, or send Sam a message on [Twitter](https://twitter.com/Sam_Twidale).
 * Geometrize is inspired by [primitive](https://github.com/fogleman/primitive), a Go library and tool created by [Michael Fogleman](https://github.com/fogleman).
 * There is up to date Doxygen documentation for [Geometrize](https://appdocs.geometrize.co.uk/index.html) and the underlying Geometrize [library](https://docs.geometrize.co.uk/).
 * Also see the Geometrize [gallery](https://gallery.geometrize.co.uk/), [bot](https://twitter.com/Geometrizer) and [tweening demo](https://tweens.geometrize.co.uk/).
