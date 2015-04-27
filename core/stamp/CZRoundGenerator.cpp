
#include "CZRoundGenerator.h"

using namespace std;

CZRoundGenerator::CZRoundGenerator(CZGLContext *ctx):CZStampGenerator(ctx)
{
	/// 创建属性
	hardness.title = "Hardness";
	//density.delegate = self;
	//(self.rawProperties)[@"density"] = density;
}

/// 拷贝一份当前生成器
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

/// 绘制图案
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