//
// Created by erik on 9/20/16.
//

#include <cmath>
#include "GridNode.h"
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/SViewFrustum.h>
#include <iostream>
#include <irrlicht/SMeshBuffer.h>

using namespace irr;

scene::GridNode::GridNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) :
        ISceneNode(parent, mgr, id),
        mMesh(new scene::SMeshBuffer())
{
    mBBox.addInternalPoint(-1e9f, -1e9f, -1e9f);
    mBBox.addInternalPoint(1e9f, 1e9f, 1e9f);

    // create new buffer

    u16 W = 40;
    u16 H = 40;

    auto buf = (SMeshBuffer*)(mMesh);
    buf->Vertices.set_used(W * H);
    u32 i=0;
    for (u16 y = 0; y < H; ++y)
    {
        for (u16 x = 0; x < W; ++x)
        {
            video::S3DVertex& v = buf->Vertices[i++];
            v.Pos.set(x-.5f*W, 0, y - .5f*H);
            v.Normal.set(0, 1, 0);
            float d = 3 * v.Pos.getLength() / std::sqrt((float)(W * W + H * H));
            u8 alpha = 255 * std::max(0.f, 1.f-d);
            v.Color = video::SColor(alpha, 128, 128, 255);
            v.TCoords.set(0, 0);
        }
    }

    buf->Indices.set_used(4*(W-1u) * (H-1) + 2*(H-1) + 2*(W-1));
    i=0;
    for(u16 y = 0; y < H; ++y)
    {
        for(u16 x = 0; x < W; ++x)
        {
            const u16 n = y * W + x;
            if(y != H-1) {
                buf->Indices[i++] = n;
                buf->Indices[i++] = n + W;
            }
            if(x != W-1) {
                buf->Indices[i++] = n;
                buf->Indices[i++] = n + 1;
            }
        }
    }
    mMesh->recalculateBoundingBox();
    mMesh->setDirty();
}

scene::GridNode::~GridNode()
{
    if(mMesh)
        mMesh->drop();
}


void scene::GridNode::OnRegisterSceneNode()
{
    if (IsVisible) {
        SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
    }

    ISceneNode::OnRegisterSceneNode();
}

void scene::GridNode::render()
{
    float scale = 10;
    auto& driver = *SceneManager->getVideoDriver();
    auto& cam = *SceneManager->getActiveCamera();

    auto mb = mMesh;

    video::SMaterial mat;
    mat.Lighting = false;
    mat.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
    driver.setMaterial(mat);
    core::matrix4 mtx;
    mtx.setScale(scale);
    auto tp = cam.getTarget();
    tp.X = std::round(tp.X/scale)*scale;
    tp.Y = std::round(tp.Y/scale)*scale;
    tp.Z = std::round(tp.Z/scale)*scale;
    mtx.setTranslation(tp);
    driver.setTransform(video::ETS_WORLD, mtx);

    driver.drawVertexPrimitiveList(mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount()/2,
                                   video::EVT_STANDARD, EPT_LINES);
}

const core::aabbox3d<f32>& scene::GridNode::getBoundingBox() const
{
    return mBBox;
}
