// @brief: 放置圆盘的杆

#include "General.h"
#include "Mesh.h"

class Pillar : public Mesh {
public:
    Pillar(const Texture &);

    Pillar(Pillar &&) = default;

    virtual void generateMesh(int iSlices, float height, float radius, float unused);

    void chosen();

    void released();

private:
    GS(float, light);
};