//
// Created by erik on 12/30/16.
//

#ifndef SPATACS_UI_SHIELDFX_H
#define SPATACS_UI_SHIELDFX_H

#include <irrlicht/ISceneManager.h>

namespace irr {
    namespace scene {
        class ShieldFx : public ISceneNode
        {
        public:
            ShieldFx(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1);

            void OnAnimate(u32 timeMs) override;
            void render() override;
            const core::aabbox3d<f32>& getBoundingBox() const override;
            void OnRegisterSceneNode() override;

            void setColor( video::SColor col );
            void setStatus(float s);

        private:
            video::SMaterial mMaterial;
            video::SColor    mColor;

            core::aabbox3d<f32> mBBox;
            float mStatus;

            void drawCircle(const core::vector3df& view, const core::vector3df& h, const core::vector3df& v, float radius,
                            float status, const video::SColor& color) const;
        };
    }
}


#endif //SPATACS_UI_SHIELDFX_H
