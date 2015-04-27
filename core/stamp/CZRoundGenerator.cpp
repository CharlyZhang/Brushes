
#include "CZRoundGenerator.h"

using namespace std;

CZRoundGenerator::CZRoundGenerator(CZGLContext *ctx):CZStampGenerator(ctx)
{
	/// ��������
	hardness.title = "Hardness";
	//density.delegate = self;
	//(self.rawProperties)[@"density"] = density;
}

/// ����һ�ݵ�ǰ������
CZRoundGenerator* CZRoundGenerator::copy()
{
	CZRoundGenerator *ret = new CZRoundGenerator(ptrGLContext);

	ret->seed = seed;
	ret->size = size;
	ret->scale = scale;
	ret->hardness = hardness;
	return ret;
}

bool CZRoundGenerator::canRandomize()
{
	return false;
}

/// ����ͼ��
void CZRoundGenerator::renderStamp(CZRandom* randomizer)
{
	if(shader == NULL && ptrGLContext)
	{
		vector<string> attributes, uniforms;
		attributes.push_back("inPosition");
		uniforms.push_back("mvpMat");
		shader = new CZShader("stamp","stamp",attributes,uniforms);
	}

	shader->begin();

	drawRadialFade(hardness.value);

	shader->end();
}