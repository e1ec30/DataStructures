/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"
#include "cs225/LUVAPixel.h"
#include "kdtree.h"
#include "mosaiccanvas.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

LUVAPixel convertToLUVA(Point<3> p) {
    return LUVAPixel(p[0], p[1], p[2]);
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas *mc = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    std::vector<Point<3>> tilePoints;
    tilePoints.reserve(theTiles.size());

    std::map<Point<3>, TileImage*> tileMap;
    mc->enableOutput = true;

    for (auto &tile: theTiles) {
        tilePoints.push_back(convertToXYZ(tile.getAverageColor()));
        tileMap.insert(std::make_pair(convertToXYZ(tile.getAverageColor()), &tile));
    }

    KDTree<3> tileTree(tilePoints);
    for (int x = 0; x < theSource.getRows(); x++) {
        for (int y = 0; y < theSource.getColumns(); y++) {
            LUVAPixel src = theSource.getRegionColor(x, y);
            Point<3> dst = tileTree.findNearestNeighbor(convertToXYZ(src));

            TileImage *dstTile = tileMap.at(dst);
            mc->setTile(x, y, dstTile);
        }
    }


    return mc;
}

