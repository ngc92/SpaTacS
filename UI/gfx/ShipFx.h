//
// Created by erik on 10/6/16.
//

#ifndef SPATACS_SHIPFX_H
#define SPATACS_SHIPFX_H



#include <irrlicht/ISceneManager.h>

namespace irr {
    namespace scene {
        class ShipFx : public ISceneNode
        {
        public:
            ShipFx(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1);

            void OnAnimate(u32 timeMs) override;
            void render() override;
            const core::aabbox3d<f32>& getBoundingBox() const override;
            void OnRegisterSceneNode() override;

            void setShip(core::vector3df pos, core::vector3df vel);
            void setColor( video::SColor col );

        private:
            core::vector3df mBasePosition;
            core::vector3df mDirection;

            video::SMaterial mMaterial;
            video::SColor    mColor;

            core::aabbox3d<f32> mBBox;
            u32 mLastTime = 0;
            float mBaseY = 0;
        };
    }
}

#endif //SPATACS_SHIPFX_H
