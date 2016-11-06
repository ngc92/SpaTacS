#ifndef SPATACS_HEALTH_BAR_H_INCLUDED
#define SPATACS_HEALTH_BAR_H_INCLUDED

#include <irrlicht/IGUIElement.h>

namespace irr
{
	namespace gui
    {
        class IGUIStaticText;

        using irr::video::SColor;
        using irr::video::ITexture;

        class HealthBar : public IGUIElement
        {
        public:
            HealthBar(gui::IGUIEnvironment* env, IGUIElement* parent, s32 id, core::recti rect);

            virtual ~HealthBar();

            virtual void draw();

            virtual void OnPostRender(u32 time);

            // set data
            void setStatus(f32 s) { mStatus = s; }
            f32 getStatus() const { return mStatus; }

            // config functions
            const SColor& getHealthColor() const { return mHealthyColor; };
            const SColor& getDamageColor() const { return mDamageColor; };
            const SColor& getTextColor() const { return mTextColor; };

            void setHealthColor(const SColor& c) { mHealthyColor = c; };
            void setDamageColor(const SColor& c) { mDamageColor = c; };
            void setTextColor(const SColor& c)   { mTextColor = c;}

            //! Writes attributes of the element.
            void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const override;

            //! Reads attributes of the element
            void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) override;


        private:
            f32 mStatus = 0.f;

            // configuration
            SColor mHealthyColor;
            SColor mDamageColor;
            SColor mTextColor;
        };
    }
}


#endif // SPATACS_HEALTH_BAR_H_INCLUDED
