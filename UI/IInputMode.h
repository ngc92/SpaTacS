//
// Created by erik on 9/22/16.
//

#ifndef SOI_IINPUTMODE_H
#define SOI_IINPUTMODE_H

#include <irrlicht/line3d.h>
#include <irrlicht/vector2d.h>
#include <irrlicht/Keycodes.h>
#include <irrlicht/IEventReceiver.h>

namespace irr
{
    namespace video
    {
        class IVideoDriver;
    }
    namespace gui
    {
        class IGUIEnvironment;
        class IGUIElement;
    }
    namespace scene
    {
        class ISceneManager;
        class ICameraSceneNode;
    }
}

namespace spatacs {
    namespace core
    {
        class GameState;
    }
    namespace cmd
    {
        class CommandManager;
    }

    namespace ui {
        class IrrlichtUI;
        class IInputMode
        {
        public:
            using ray_t = irr::core::line3df;

            void setMainUI( IrrlichtUI* mainui ) ;

            virtual void onMouseMove(  ray_t ray ) {}
            virtual void onLeftClick(  ray_t ray ) {}
            virtual void onRightClick(ray_t ray, const irr::SEvent::SMouseInput& event) {}
            virtual void onWheel( float scroll )   {}
            virtual void onKeyPress( irr::EKEY_CODE key )   { };
            virtual void onKeyRelease( irr::EKEY_CODE key ) { };;

            virtual void step() { };

        protected:
            virtual void init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr)
            { };

            irr::core::vector2di getScreenPosition(const irr::core::vector3df pos3d);
            irr::scene::ICameraSceneNode* getCamera();

            const core::GameState& state() const;
            cmd::CommandManager& getCmdMgr();


            IrrlichtUI* getMainUI() { return mMainUI; }
        private:
            IrrlichtUI* mMainUI;
        };
    }
}


#endif //SOI_IINPUTMODE_H
