
#include "ogldev_math_3d.h"
#include "ogldev_gui_texture.h"


#define GUI_TEXTURE_UNIT       GL_TEXTURE0
#define GUI_TEXTURE_UNIT_INDEX 0


GUITexture::~GUITexture()
{
}


void GUITexture::Init(int x, int y, int Width, int Height)
{
    m_screenQuad.Init(x, y, Width, Height);
    m_guiTexTech.Init();
    m_guiTexTech.Enable();
    m_guiTexTech.SetTextureUnit(GUI_TEXTURE_UNIT_INDEX);
}


void GUITexture::Render(GLuint Texture)
{
    glActiveTexture(GUI_TEXTURE_UNIT);
    glBindTexture(GL_TEXTURE_2D, Texture);

    m_guiTexTech.Enable();

    m_screenQuad.Render();
}
