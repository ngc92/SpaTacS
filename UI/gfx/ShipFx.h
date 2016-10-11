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

            void setColor( video::SColor col );
            void setShieldStatus(float s);
            void setHullStatus(float s);

        private:
            video::SMaterial mMaterial;
            video::SColor    mColor;

            core::aabbox3d<f32> mBBox;
            float mShieldStatus;
            float mHullStatus;

            void drawCircle(const core::vector3df& view, const core::vector3df& h, const core::vector3df& v, float radius,
                                        float status, const video::SColor& color) const;
        };
    }
}

#endif //SPATACS_SHIPFX_H
