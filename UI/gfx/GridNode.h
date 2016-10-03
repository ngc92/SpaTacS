//
// Created by erik on 9/20/16.
//

#ifndef SOI_GRIDNODE_H
#define SOI_GRIDNODE_H

#include <irrlicht/ISceneNode.h>

namespace irr
{
    namespace scene
    {
        class IMeshBuffer;

        class GridNode : public ISceneNode
        {
        public:
            GridNode(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1);

            ~GridNode();
            virtual void render() override;

            virtual const core::aabbox3d<f32>& getBoundingBox() const override;

            void OnRegisterSceneNode();
        private:
            core::aabbox3d<f32> mBBox;
            scene::IMeshBuffer* mMesh;
        };
    }
}

#endif //SOI_GRIDNODE_H
