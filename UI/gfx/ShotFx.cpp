//
// Created by erik on 9/21/16.
//

#include "ShotFx.h"
#include <irrlicht/IVideoDriver.h>
#include <iostream>

using namespace irr;

void scene::ShotFx::render()
{
    video::SMaterial mat;
    mat.Lighting = false;
    SceneManager->getVideoDriver()->setMaterial(mat);
    SceneManager->getVideoDriver()->setTransform(video::ETS_WORLD, core::matrix4());
    auto p = getAbsolutePosition();
    SceneManager->getVideoDriver()->draw3DLine( p - 0.1*mDirection, p, mColor);
}

const core::aabbox3d<f32>& scene::ShotFx::getBoundingBox() const
{
    return mBBox;
}

scene::ShotFx::ShotFx(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) :
        ISceneNode(parent, mgr, id), mColor(255, 255, 0, 0)
{
    mBBox.reset( core::vector3df(0,0,0) );
}

void scene::ShotFx::setShot(core::vector3df vel)
{
    mDirection = vel;
    mBBox.addInternalPoint(vel);
}

void scene::ShotFx::OnAnimate(u32 timeMs)
{
    ISceneNode::OnAnimate(timeMs);
}

void scene::ShotFx::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}

void scene::ShotFx::setColor(video::SColor col)
{
    mColor = col;
}

const video::SColor& scene::ShotFx::getColor() const
{
    return mColor;
}
