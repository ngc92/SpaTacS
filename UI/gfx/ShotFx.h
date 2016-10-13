//
// Created by erik on 9/21/16.
//

#ifndef SOI_SHOTFX_H
#define SOI_SHOTFX_H

#include <irrlicht/ISceneManager.h>

namespace irr {
    namespace scene {
        class ShotFx : public ISceneNode
        {
        public:
            ShotFx(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1);

            void OnAnimate(u32 timeMs) override;
            void render() override;
            const core::aabbox3d<f32>& getBoundingBox() const override;
            void OnRegisterSceneNode() override;

            void setShot(core::vector3df vel);

            void setColor( video::SColor col );
            const video::SColor& getColor() const;

        private:
            core::vector3df mDirection;
            core::aabbox3d<f32> mBBox;
            video::SColor mColor;
        };
    }
}


#endif //SOI_SHOTFX_H
