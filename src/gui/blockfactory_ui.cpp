/** ICP Project 2017/2018: BlockEditor
 * @file blockfactory_ui.cpp
 * @brief Block type specific creation
 * @author Tomáš Pazdiora (xpazdi02)
 * @author Michal Pospíšil (xpospi95)
 */


#include "blockfactory_ui.h"
#include "port_ui.h"
#include "graph_ui.h"
#include "block_ui.h"

#include "../core/blocks/scalar_io.h"
#include "../core/blocks/vector_io.h"
#include "../core/blocks/vector3D_io.h"
#include "../core/blocks/matrix.h"
#include "../core/blocks/vectoraddblock.h"
#include "../core/blocks/vectoraddblock3D.h"
#include "../core/blocks/vectordotproductblock.h"
#include "../core/blocks/scalaraddblock.h"
#include "../core/blocks/scalarsubblock.h"
#include "../core/blocks/scalarmulblock.h"

BlockBase *BlockFactoryUI::AllocBlock(BlockType t)
{
	GraphUI *gp = static_cast<GraphUI*>(&g);
	BlockBase *b;
	switch (t) {
	case SCAL_INPUT:
		b = new InputBlockUI<ScalarInput>(BlockUI<ScalarInput>(ScalarInput(g), gp), gp);
		break;
	case SCAL_OUTPUT:
		b = new OutputBlockUI<ScalarOutput>(BlockUI<ScalarOutput>(ScalarOutput(g), gp), gp);
		break;
	case VECTOR_INPUT:
		b = new InputBlockUI<VectorInput>(BlockUI<VectorInput>(VectorInput(g), gp), gp);
		break;
    case VECTOR3D_INPUT:
        b = new InputBlockUI<Vector3DInput>(BlockUI<Vector3DInput>(Vector3DInput(g), gp), gp);
        break;
    case VECTOR_OUTPUT:
        b = new OutputBlockUI<VectorOutput>(BlockUI<VectorOutput>(VectorOutput(g), gp), gp);
        break;
    case VECTOR3D_OUTPUT:
        b = new OutputBlockUI<Vector3DOutput>(BlockUI<Vector3DOutput>(Vector3DOutput(g), gp), gp);
		break;
	case MAT2_INPUT:
		b = new InputBlockUI<MatrixInput>(BlockUI<MatrixInput>(MatrixInput(g), gp), gp);
		break;
	case MAT2_OUTPUT:
		b = new OutputBlockUI<MatrixOutput>(BlockUI<MatrixOutput>(MatrixOutput(g), gp), gp);
		break;
    case MAT3_INPUT:
        b = new InputBlockUI<Matrix3x3Input>(BlockUI<Matrix3x3Input>(Matrix3x3Input(g), gp), gp);
        break;
    case MAT3_OUTPUT:
        b = new OutputBlockUI<Matrix3x3Output>(BlockUI<Matrix3x3Output>(Matrix3x3Output(g), gp), gp);
        break;
	case VECTOR_ADD:
		b = new BlockUI<VectorAddBlock>(VectorAddBlock(g), gp);
		break;
    case VECTOR_ADD3D:
        b = new BlockUI<VectorAddBlock3D>(VectorAddBlock3D(g), gp);
        break;
    case VECTOR_DOTPRODUCT:
		b = new BlockUI<VectorDotProductBlock>(VectorDotProductBlock(g), gp);
        break;
    case SCALAR_ADD:
		b = new BlockUI<ScalarAddBlock>(ScalarAddBlock(g), gp);
        break;
    case SCALAR_SUB:
		b = new BlockUI<ScalarSubBlock>(ScalarSubBlock(g), gp);
        break;
    case SCALAR_MUL:
		b = new BlockUI<ScalarMulBlock>(ScalarMulBlock(g), gp);
        break;
    case MAT_ADD:
		b = new BlockUI<MatrixAddBlock>(MatrixAddBlock(g), gp);
        break;
    case MAT3_ADD:
        b = new BlockUI<Matrix3x3AddBlock>(Matrix3x3AddBlock(g), gp);
        break;
    case MAT_MUL:
		b = new BlockUI<MatrixMulBlock>(MatrixMulBlock(g), gp);
        break;
    case MAT3_MUL:
        b = new BlockUI<Matrix3x3MulBlock>(Matrix3x3MulBlock(g), gp);
        break;
	case MAT_MUL_VEC:
		b = new BlockUI<MatMulVecBlock>(MatMulVecBlock(g), gp);
		break;
	default:
		return nullptr;
	}
	blocks.push_back(b);
	return b;
}

BlockFactoryUI::BlockFactoryUI(GraphUI &g)
	: BlockFactory(g), block_parent(&g) { }
