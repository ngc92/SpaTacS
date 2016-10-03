//
// Created by erik on 9/21/16.
//

#include "ShotFx.h"
#include <irrlicht/IVideoDriver.h>

using namespace irr;

void scene::ShotFx::render()
{
    video::SMaterial mat;
    mat.Lighting = false;
    SceneManager->getVideoDriver()->setMaterial(mat);
    SceneManager->getVideoDriver()->setTransform(video::ETS_WORLD, core::matrix4());
    SceneManager->getVideoDriver()->draw3DLine( mBasePosition - 0.1*mDirection, mBasePosition, video::SColor(255, 255, 0, 0 ));
}

const core::aabbox3d<f32>& scene::ShotFx::getBoundingBox() const
{
    return mBBox;
}

scene::ShotFx::ShotFx(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) :
        ISceneNode(parent, mgr, id)
{

}

void scene::ShotFx::setShot(core::vector3df pos, core::vector3df vel)
{
    mBasePosition = pos;
    mDirection = vel;
    mBBox.reset( mBasePosition );
    mBBox.addInternalPoint( mBasePosition + mDirection );
}

void scene::ShotFx::OnAnimate(u32 timeMs)
{
    if(mLastTime != 0)
    {
        mBasePosition += (timeMs - mLastTime) / 1000.f * mDirection;
        mBBox.addInternalPoint( mBasePosition + mDirection );
    }
    mLastTime = timeMs;

    ISceneNode::OnAnimate(timeMs);
}

void scene::ShotFx::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}
