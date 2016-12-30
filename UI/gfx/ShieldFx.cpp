//
// Created by erik on 12/30/16.
//

#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/SViewFrustum.h>
#include <cmath>
#include <iostream>
#include "ShieldFx.h"

using namespace irr;
using irr::scene::ShieldFx;

ShieldFx::ShieldFx(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
        :ISceneNode(parent, mgr, id)
{
    mBBox.reset( core::vector3df(-10, -10, -10) );
    mBBox.addInternalPoint( core::vector3df(10, 10, 10) );
    mMaterial.Lighting = false;
}

void ShieldFx::OnAnimate(u32 timeMs)
{
    ISceneNode::OnAnimate(timeMs);
}

void ShieldFx::render()
{
// parts of the code copied from ParticleSystemSceneNode
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    ICameraSceneNode* camera = SceneManager->getActiveCamera();
    if (!camera || !driver)
        return;

    driver->setMaterial(mMaterial);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    const core::matrix4 &m = camera->getViewFrustum()->getTransform( video::ETS_VIEW );

    float f = 0.75f; /// \todo depend on radius

    const core::vector3df view ( -m[2], -m[6] , -m[10] );
    const core::vector3df h ( m[0] * f, m[4] * f, m[8] * f );
    const core::vector3df v ( m[1] * f, m[5] * f, m[9] * f );

    drawCircle(view, h, v, 2.25, mStatus, mColor);

}

void ShieldFx::drawCircle(const core::vector3df& view, const core::vector3df& h, const core::vector3df& v, float radius,
                        float status, const video::SColor& color) const
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    constexpr uint32_t vtx_ct = 13;
    video::S3DVertex svtx[vtx_ct];
    u16 idx[vtx_ct];
    for(uint16_t i = 0; i < vtx_ct; ++i)
    {
        float a = status * 2 * i * 3.1415f / (vtx_ct - 1) + 3.1415f / 2;
        svtx[i].Pos = radius*(std::sin(a) * h + std::cos(a) * v);
        svtx[i].Color = color;
        svtx[i].Normal = -view;
        idx[i] = i;
    }
    driver->drawVertexPrimitiveList(svtx, vtx_ct, idx, vtx_ct-1, video::EVT_STANDARD, EPT_LINE_STRIP);
}


const core::aabbox3d<f32>& ShieldFx::getBoundingBox() const
{
    return mBBox;
}

void ShieldFx::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}

void ShieldFx::setColor(video::SColor col)
{
    mColor = col;
}

void ShieldFx::setStatus(float s)
{
    if(s >= 0 && s <= 1)
        mStatus = s;
}
