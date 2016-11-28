#ifndef GEOMETRIZE_H
#define GEOMETRIZE_H

#include "state.h"
#include "bitmap/bitmapdata.h"
#include "bitmap/rgba.h"
#include "shape/shape.h"
#include "shape/shapetypes.h"

namespace geometrize
{

class State;

/**
 * The core functions for Geometrize.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */

/**
 * @brief energy Calculates a measure of the improvement adding the shape provides - lower energy is better.
 * @param shape The shape to check.
 * @param alpha The alpha of the shape.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @param score The score.
 * @return The energy measure.
 */
float energy(const Shape& shape, const int alpha, const BitmapData& target, const BitmapData& current, const BitmapData& buffer, const float score)
{
    // TODO
    return 0.0f;
}

/**
 * @brief computeColor Calculates the color of the scanlines.
 * @param target The target image.
 * @param current The current image.
 * @param lines The scanlines.
 * @param alpha The alpha of the scanline.
 * @return The color of the scanlines.
 */
rgba computeColor(const BitmapData& target, const BitmapData& current, const std::vector<Scanline>& lines, const int alpha)
{
    // TODO
    return rgba{0, 0, 0, 0};
}

/**
 * @brief drawLines Draws scanlines onto an image.
 * @param image The image to be drawn to.
 * @param color The color of the scanlines.
 * @param lines The scanlines to draw.
 */
void drawLines(const BitmapData& image, const rgba color, const std::vector<Scanline>& lines)
{
    // TODO
}

/**
 * @brief copyLines Copies source pixels to a destination defined by a set of scanlines.
 * @param destination The destination bitmap to copy the lines to.
 * @param source The source bitmap to copy the lines from.
 * @param lines The scanlines that comprise the source to destination copying mask.
 */
void copyLines(const BitmapData& destination, const BitmapData& source, const std::vector<Scanline>& lines)
{
    // TODO
}

/**
 * @brief differenceFull Calculates the root-mean-square error between two bitmaps.
 * @param first The first bitmap.
 * @param second The second bitmap.
 * @return The difference/error measure between the two bitmaps.
 */
float differenceFull(const BitmapData& first, const BitmapData& second)
{
    // TODO
    return 0.0f;
}

/**
 * @brief differencePartial Calculates the root-mean-square error between the parts of the two bitmaps within the scanline mask.
 * This is for optimization purposes, it lets us calculate new error values only for parts of the image we know have changed.
 * @param target The target bitmap.
 * @param before The bitmap before the change.
 * @param after The bitmap after the change.
 * @param score The score.
 * @param lines The scanlines.
 * @return The difference/error between the two bitmaps, masked by the scanlines.
 */
float differencePartial(const BitmapData& target, const BitmapData& before, const BitmapData& after, const float score, const std::vector<Scanline>& lines)
{
    // TODO
    return 0.0f;
}

/**
 * @brief bestHillClimbState Gets the best state using a hill climbing algorithm.
 * @param shapeTypes The types of shape to use.
 * @param alpha The opacity of the shape.
 * @param n The number of random states to generate.
 * @param age The number of hillclimbing steps.
 * @param m The number of iterations for the overall algorithm.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best state acquired from hill climbing i.e. the one with the lowest energy.
 */
State* bestHillClimbState(const ShapeTypes shapeTypes, const int alpha, const int n, const int age, const int m, const BitmapData& target, const BitmapData& current, const BitmapData& buffer)
{
    // TODO
    return nullptr;
}

/**
 * @brief bestRandomState Gets the best state using a random algorithm.
 * @param shapeTypes The types of shape to use.
 * @param alpha The opacity of the shape.
 * @param n The number of states to try.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best random state i.e. the one with the lowest energy.
 */
State* bestRandomState(const ShapeTypes shapeTypes, const int alpha, const int n, const BitmapData& target, const BitmapData& current, const BitmapData& buffer)
{
    // TODO
    return nullptr;
}

/**
 * @brief hillClimb Hill climbing optimization algorithm, attempts to minimize energy (the error/difference).
 * @param state The state to optimize.
 * @param maxAge The maximum age.
 * @return The best state found from hillclimbing.
 */
State* hillClimb(const State& state, const int maxAge)
{
    // TODO
    return nullptr;
}

/**
 * @brief getAverageImageColor Computes the average RGB color of the pixels in the image.
 * @param image The image whose average color will be calculated.
 * @return The average RGB color of the image, RGBA8888 format. Alpha is set to opaque (255).
 */
rgba getAverageImageColor(const BitmapData& image)
{
    // TODO
    return{0, 0, 0, 0};
}

}

#endif // GEOMETRIZE_H
