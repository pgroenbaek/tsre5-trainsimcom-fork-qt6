/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHADER_H
#define	SHADER_H

#include <QOpenGLShaderProgram>

class Shader : public QOpenGLShaderProgram {
public:
    Shader();
    virtual ~Shader();
    int shaderProgram;
    int vertexPositionAttribute;
    int textureCoordAttribute;
    int pShadowMatrixUniform;
    int pShadow2MatrixUniform;
    int pMatrixUniform;
    int fMatrixUniform;
    int mvMatrixUniform;
    int msMatrixUniform;
    int samplerUniform;
    int lod;
    int sun;
    int brightness;
    int skyColor;
    int skyLight;
    int shaderAlpha;
    int shaderAlphaTest;
    int shaderTextureEnabled;
    int shaderShapeColor;
    int shaderEnableNormals;
    int shaderDiffuseColor;
    int shaderAmbientColor;
    int shaderSpecularColor;
    int shaderLightDirection;
    int shaderSecondTexEnabled;
    int shaderShadowsEnabled;
    int shaderBrightness;
    int shaderFogDensity;
    int shadow1Res;
    int shadow1Bias;
    int shadow2Res;
    int shadow2Bias;
private:

};

#endif	/* SHADER_H */

