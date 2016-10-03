//
// Created by erik on 9/20/16.
//

#include <iostream>
#include "LocationPlotter3D.h"
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/SViewFrustum.h>
#include <irrlicht/S3DVertex.h>
#include <irrlicht/SMeshBuffer.h>
#include <cmath>

using namespace irr;
using scene::LocationPlotter3D;

LocationPlotter3D::LocationPlotter3D(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
                                     const irr::core::vector3df& position, const irr::core::vector3df& rotation,
                                     const irr::core::vector3df& scale) : ISceneNode(parent, mgr, id, position,
                                                                                     rotation, scale)
{
    mMaterial.Lighting = false;
}

void LocationPlotter3D::render()
{
    // parts of the code copied from ParticleSystemSceneNode
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    ICameraSceneNode* camera = SceneManager->getActiveCamera();
    if (!camera || !driver)
        return;

    const core::matrix4 &m = camera->getViewFrustum()->getTransform( video::ETS_VIEW );
    const core::vector3df view ( -m[2], -m[6] , -m[10] );

    driver->setMaterial(mMaterial);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    mBaseY = std::round(camera->getTarget().Y / 10) * 10;

    float f = 1; /// \todo depend on radius

    for(auto& loc : mPositions)
    {
        auto pos = loc.pos;
        auto col = mColors.at(loc.col);

        core::vector3df base{pos.X, mBaseY, pos.Z};
        driver->draw3DLine( pos, base );
        const core::vector3df h ( m[0] * f, m[4] * f, m[8] * f );
        const core::vector3df v ( m[1] * f, m[5] * f, m[9] * f );

        video::S3DVertex vertices[] = {{pos + h + v, -view, col, {0.f, 0.f}},
                                       {pos + h - v, -view, col, {0.f, 1.f}},
                                       {pos - h - v, -view, col, {1.f, 1.f}},
                                       {pos - h + v, -view, col, {1.f, 0.f}}
        };

        u16 indices[] = {0, 1, 2, 0, 2, 3};
        driver->drawIndexedTriangleList(&vertices[0], 4, &indices[0], 2);
    }
}

void LocationPlotter3D::setLocations(std::vector<Location> loc)
{
    if(loc.size() > 0)
        mBoundingBox.reset(loc[0].pos);
    mPositions = std::move(loc);
    for(auto& p : mPositions)
        mBoundingBox.addInternalPoint(p.pos);
}

void LocationPlotter3D::setColor(u8 idx, video::SColor col)
{
    if(mColors.size() <= idx)
        mColors.resize( idx + 1 );
    mColors.at(idx) = col;
}

const irr::core::aabbox3d<irr::f32>& LocationPlotter3D::getBoundingBox() const
{
    return mBoundingBox;
}

void LocationPlotter3D::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

u32 LocationPlotter3D::getMaterialCount() const
{
    return 1;
}

video::SMaterial& LocationPlotter3D::getMaterial(u32 num)
{
    return mMaterial;
}

void LocationPlotter3D::OnAnimate(u32 timeMs)
{
    if (mLastTime != 0) {
        for(auto& loc : mPositions) {
            loc.pos += (timeMs - mLastTime) / 1000.f * loc.vel;
            mBoundingBox.addInternalPoint(loc.pos);
        }
    }
    mLastTime = timeMs;

    ISceneNode::OnAnimate(timeMs);
}

void LocationPlotter3D::setBasePlane(float y)
{
    mBaseY = y;
}
