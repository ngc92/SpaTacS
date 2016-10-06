//
// Created by erik on 10/6/16.
//

#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/SViewFrustum.h>
#include <cmath>
#include "ShipFx.h"

using namespace irr;
using irr::scene::ShipFx;

ShipFx::ShipFx(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
:ISceneNode(parent, mgr, id)
{
    mMaterial.Lighting = false;
}

void ShipFx::OnAnimate(u32 timeMs)
{
    if(mLastTime != 0)
    {
        mBasePosition += (timeMs - mLastTime) / 1000.f * mDirection;
        mBBox.addInternalPoint( mBasePosition + mDirection );
    }
    mLastTime = timeMs;

    ISceneNode::OnAnimate(timeMs);
}

void ShipFx::render()
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

    auto& pos = mBasePosition;
    auto& col = mColor;

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

const core::aabbox3d<f32>& ShipFx::getBoundingBox() const
{
    return mBBox;
}

void ShipFx::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}

void ShipFx::setShip(core::vector3df pos, core::vector3df vel)
{
    mBasePosition = pos;
    mDirection = vel;
    mBBox.reset( mBasePosition );
    mBBox.addInternalPoint( mBasePosition + mDirection );
    auto b0 = mBasePosition;
    b0.Y  = mBaseY;
    mBBox.addInternalPoint( b0 );
}

void ShipFx::setColor(video::SColor col)
{
    mColor = col;
}
