/** VUT FIT ICP 2020/2021
 * @file calculator.cpp
 * @brief Súbor obsahuje funkcie pre prácu so základnými matematickými operáciami
 * @date 3.4.2021
 * @author Tomáš Zaťko  - xzatko02
 * @author Martin Rakús - xrakus04
 */

#include "../types/types.h"
#include "../blocks.h"
#include "calculator.h"
#include "math.h"
#include <cmath>


// blok pre načítanie vstupu čísla
MathInput::MathInput(Graph &g)
    : BlockBase(g, MATH_INPUT, "Vstup",
{}, {OutPort(*this, one_number(), "")}) { }


// blok pre výstup čísla
MathOutput::MathOutput(Graph &g)
    : BlockBase(g, MATH_OUTPUT, "Výstup",
    {InPort(*this, one_number(), "")}, {}) { }


void MathInput::Compute() { }
void MathOutput::Compute() { }


// blok pre sčítanie dvoch čísel
MathAddBlock::MathAddBlock(Graph &g)
    : BlockBase(g, MATH_ADD, BLOCK_NAME.at(MATH_ADD),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet sčítania dvoch čísel
void MathAddBlock::Compute(){
    this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] + this->Input(1)["Hodnota"];
}


// blok pre odčítanie dvoch čísel
MathSubBlock::MathSubBlock(Graph &g)
    : BlockBase(g, MATH_SUB, BLOCK_NAME.at(MATH_SUB),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet odčítania dvoch čísel
void MathSubBlock::Compute(){
    this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] - this->Input(1)["Hodnota"];
}


// blok pre násobenie dvoch čísel
MathMulBlock::MathMulBlock(Graph &g)
    : BlockBase(g, MATH_MUL, BLOCK_NAME.at(MATH_MUL),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet násobenia dvoch čísel
void MathMulBlock::Compute(){
    this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] * this->Input(1)["Hodnota"];
}


// blok pre delenie dvoch čísel
MathDivBlock::MathDivBlock(Graph &g)
    : BlockBase(g, MATH_DIV, BLOCK_NAME.at(MATH_DIV),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet delenia dvoch čísel
void MathDivBlock::Compute(){
    this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] / this->Input(1)["Hodnota"];
}


// blok pre druhú odmocninu čísla
MathSqrtBlock::MathSqrtBlock(Graph &g)
    : BlockBase(g, MATH_SQRT, BLOCK_NAME.at(MATH_SQRT),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet druhej odmocniny čisla
void MathSqrtBlock::Compute(){
    this->Output(0)["Hodnota"] = sqrt(this->Input(0)["Hodnota"]);
}


// blok pre výpočet absolútnej hodnoty čísla
MathAbsBlock::MathAbsBlock(Graph &g)
    : BlockBase(g, MATH_ABS, BLOCK_NAME.at(MATH_ABS),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet druhej odmocniny čisla
void MathAbsBlock::Compute(){
    this->Output(0)["Hodnota"] = abs(this->Input(0)["Hodnota"]);
}


// blok pre vyhodnotenie väčšieho čísla z dvoch - GT - greater than
MathGTBlock::MathGTBlock(Graph &g)
    : BlockBase(g, MATH_GT, BLOCK_NAME.at(MATH_GT),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// vyhodnotenie väčšieho čísla z dvoch - GT - greater than
void MathGTBlock::Compute(){
    if (this->Input(0)["Hodnota"] > this->Input(1)["Hodnota"]){
        this->Output(0)["Hodnota"] = -this->Input(0)["Hodnota"];
        this->Output(0)["Hodnota"] = -this->Output(0)["Hodnota"];
    }
    else{
        this->Output(0)["Hodnota"] = -this->Input(1)["Hodnota"];
        this->Output(0)["Hodnota"] = -this->Output(0)["Hodnota"];
    }
}


// blok pre vyhodnotenie menšieho čísla z dvoch - LT - less than
MathLTBlock::MathLTBlock(Graph &g)
    : BlockBase(g, MATH_LT, BLOCK_NAME.at(MATH_LT),
    {
        InPort(*this, one_number(), "Číslo A"),
        InPort(*this, one_number(), "Číslo B")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// vyhodnotenie menšieho čísla z dvoch - LT - less than
void MathLTBlock::Compute(){
    if (this->Input(0)["Hodnota"] < this->Input(1)["Hodnota"]){
        this->Output(0)["Hodnota"] = -this->Input(0)["Hodnota"];
        this->Output(0)["Hodnota"] = -this->Output(0)["Hodnota"];
    }
    else{
        this->Output(0)["Hodnota"] = -this->Input(1)["Hodnota"];
        this->Output(0)["Hodnota"] = -this->Output(0)["Hodnota"];
    }
}


// blok pre výpočet sinusu
MathSinBlock::MathSinBlock(Graph &g)
    : BlockBase(g, MATH_SIN, BLOCK_NAME.at(MATH_SIN),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet sinusu
void MathSinBlock::Compute(){
        this->Output(0)["Hodnota"] = sin(this->Input(0)["Hodnota"]);
}


// blok pre výpočet cosinusu
MathCosBlock::MathCosBlock(Graph &g)
    : BlockBase(g, MATH_COS, BLOCK_NAME.at(MATH_COS),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// výpočet cosinusu
void MathCosBlock::Compute(){
        this->Output(0)["Hodnota"] = cos(this->Input(0)["Hodnota"]);
}


// blok pre inkrement
MathIncBlock::MathIncBlock(Graph &g)
    : BlockBase(g, MATH_INC, BLOCK_NAME.at(MATH_INC),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// vykonanie inkrementu
void MathIncBlock::Compute(){
        this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] + 1;
}


// blok pre dekrement
MathDecBlock::MathDecBlock(Graph &g)
    : BlockBase(g, MATH_DEC, BLOCK_NAME.at(MATH_DEC),
    {
        InPort(*this, one_number(), "Číslo A")
    },
    {
        OutPort(*this, one_number(), "Výstup")
    }
    ){}


// vykonanie inkrementu
void MathDecBlock::Compute(){
        this->Output(0)["Hodnota"] = this->Input(0)["Hodnota"] - 1;
}
