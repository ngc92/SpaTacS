//
// Created by erik on 10/16/16.
//

#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ISceneManager.h>
#include "MultiLineNode.h"

using namespace irr::scene;

MultiLineNode::MultiLineNode(ISceneNode* parent, ISceneManager* mgr, irr::s32 id)
    : ISceneNode(parent, mgr, id)
{

}

void MultiLineNode::render()
{
    video::SMaterial mat;
    mat.Lighting = false;
    SceneManager->getVideoDriver()->setMaterial(mat);
    SceneManager->getVideoDriver()->setTransform(video::ETS_WORLD, irr::core::matrix4());

    for(auto& l : mLines )
    {
        SceneManager->getVideoDriver()->draw3DLine( l.start, l.end, l.color );
    }
}

void MultiLineNode::OnRegisterSceneNode()
{
    if (IsVisible) {
        SceneManager->registerNodeForRendering(this, ESNRP_SOLID);
    }

    ISceneNode::OnRegisterSceneNode();
}

void MultiLineNode::clear()
{
    mBBox.reset(0,0,0);
    mLines.clear();
}

void MultiLineNode::addLine(irr::core::vector3df s, irr::core::vector3df e, video::SColor color)
{
    mLines.push_back(LineEntry{s, e, color});
    mBBox.addInternalPoint(s);
    mBBox.addInternalPoint(e);
}

const irr::core::aabbox3d<irr::f32>& MultiLineNode::getBoundingBox() const
{
    return mBBox;
}
