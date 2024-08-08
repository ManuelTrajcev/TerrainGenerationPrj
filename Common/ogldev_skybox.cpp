
#include "ogldev_skybox.h"
#include "ogldev_pipeline.h"
#include "ogldev_util.h"
#include "ogldev_basic_mesh.h"

SkyBox::SkyBox()
{
    m_pSkyboxTechnique = NULL;
    m_pCubemapTex = NULL;
    m_pMesh = NULL;
}


SkyBox::~SkyBox()
{
    SAFE_DELETE(m_pSkyboxTechnique);
    SAFE_DELETE(m_pCubemapTex);
    SAFE_DELETE(m_pMesh);
}


bool SkyBox::Init(const string& Directory,
                  const string& PosXFilename,
                  const string& NegXFilename,
                  const string& PosYFilename,
                  const string& NegYFilename,
                  const string& PosZFilename,
                  const string& NegZFilename)
{
    m_pSkyboxTechnique = new SkyboxTechnique();

    if (!m_pSkyboxTechnique->Init()) {
        printf("Error initializing the skybox technique\n");
        return false;
    }

    m_pSkyboxTechnique->Enable();
    m_pSkyboxTechnique->SetTextureUnit(0);

    m_pCubemapTex = new CubemapTexture(Directory,
                                       PosXFilename,
                                       NegXFilename,
                                       PosYFilename,
                                       NegYFilename,
                                       PosZFilename,
                                       NegZFilename);

    if (!m_pCubemapTex->Load()) {
        return false;
    }

    m_pMesh = new BasicMesh();

    return m_pMesh->LoadMesh("../Content/box.obj");
}


void SkyBox::Render(const BasicCamera& Camera)
{
    m_pSkyboxTechnique->Enable();

    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);

    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);

    glDepthFunc(GL_LEQUAL);

    static float r = 0.0f;
    Matrix4f Rotation;
    Rotation.InitRotateTransform(0.0f, r, 0.0f);
    r += 0.01f;

    Matrix4f View;
    View.InitCameraTransform(Vector3f(0.0f, 0.0f, 0.0f), Camera.GetTarget(), Camera.GetUp());
    Matrix4f Proj;
    Proj.InitPersProjTransform(Camera.GetPersProjInfo());
    Matrix4f WVP = Proj * View * Rotation;
    m_pSkyboxTechnique->SetWVP(WVP);
    m_pCubemapTex->Bind(GL_TEXTURE0);
    m_pMesh->Render();

    glCullFace(OldCullFaceMode);

    glDepthFunc(OldDepthFuncMode);
}
