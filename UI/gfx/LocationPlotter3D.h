//
// Created by erik on 9/20/16.
//

#ifndef SOI_LOCATIONPLOTTER3D_H
#define SOI_LOCATIONPLOTTER3D_H

#include <irrlicht/ISceneNode.h>
#include <vector>

namespace irr
{
namespace scene
{
    struct Location
    {
        core::vector3df pos;
        core::vector3df vel;
        u8 col;
    };

class LocationPlotter3D : public ISceneNode
{
public:
    LocationPlotter3D(ISceneNode* parent = nullptr, ISceneManager* mgr = nullptr, s32 id = -1,
    const core::vector3df& position = core::vector3df(0, 0, 0),
    const core::vector3df& rotation = core::vector3df(0, 0, 0),
    const core::vector3df& scale    = core::vector3df(1, 1, 1));

    virtual void render() override;

    virtual const core::aabbox3d<f32>& getBoundingBox() const override;

    void setLocations(std::vector<Location> pos);
    void setColor(u8 idx, video::SColor col);

    void setBasePlane( float y );

    virtual void OnAnimate(u32 timeMs) override;

    u32 getMaterialCount() const override;
    video::SMaterial& getMaterial(u32 num) override;

    void OnRegisterSceneNode() override;
private:
    irr::core::aabbox3df mBoundingBox;
    std::vector<Location> mPositions;
    std::vector<video::SColor> mColors;
    video::SMaterial mMaterial;

    irr::u32 mLastTime = 0;
    float mBaseY = 0;
};
}
}


#endif //SOI_LOCATIONPLOTTER3D_H
