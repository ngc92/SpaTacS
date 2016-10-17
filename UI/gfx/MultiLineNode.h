//
// Created by erik on 10/16/16.
//

#ifndef SPATACS_MULTILINENODE_H
#define SPATACS_MULTILINENODE_H

#include <irrlicht/ISceneNode.h>
#include <irrlicht/line3d.h>
#include <vector>

namespace irr
{
namespace scene
{
    class MultiLineNode: public ISceneNode
    {
    public:
        MultiLineNode(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1);

        void render() override;
        void OnRegisterSceneNode() override;
        const core::aabbox3d<f32>& getBoundingBox() const override;

        // lines
        void clear();
        void addLine(irr::core::vector3df s, irr::core::vector3df e, video::SColor color);
    private:
        struct LineEntry
        {
            irr::core::vector3df start;
            irr::core::vector3df end;
            irr::video::SColor color;
        };
        std::vector<LineEntry> mLines;
        core::aabbox3df mBBox;
    };
}
}

#endif //SPATACS_MULTILINENODE_H
