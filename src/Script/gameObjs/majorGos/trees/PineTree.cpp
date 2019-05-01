/*
 * ========================= PineTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/trees/PineTree.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 
#include "Script/gameObjs/create_go_oth.h"


using namespace std::placeholders;

#include "debug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------

namespace{//-------------- namespace ------------------//

    //--- 将所有 oakId 分类，方便分配 ---
    std::vector<int> ids_age1   { 0, 1 };
    //---
    std::vector<int> ids_age2   { 2, 3 };
    //---
    std::vector<int> ids_age3   { 4, 5, 6, 7 };
    //---
    std::vector<int> ids_age4   { 7 }; //- 暂无


    //===== funcs =====//
    int apply_a_oakId( int _age, float _fieldWeight );


}//------------------ namespace: end ------------------//


/* ===========================================================
 *                 init_in_autoMod
 * -----------------------------------------------------------
 */
void PineTree::init_in_autoMod(  GameObj *_goPtr,
                                const IntVec2 &_mpos,
					            float _fieldWeight,
					            const Altitude &_alti,
					            const Density &_density ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(PineTree_PvtBinary) );
    pvtBp = (PineTree_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

        pvtBp->age = gameObjs::apply_treeAge_by_density( _density );
        pvtBp->oakId = apply_a_oakId( pvtBp->age, _fieldWeight );
        //...
        

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &PineTree::OnRenderUpdate, &pineTree, _1 );   
    goPtr->LogicUpdate  = std::bind( &PineTree::OnLogicUpdate,  &pineTree, _1 );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &PineTree::OnActionSwitch, &pineTree, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
                //- 当前树木只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goPtr->species = PineTree::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::AbsFixed; //- 无法移动
    goPtr->weight = 50.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_1 );   //- 树木一律无法移动
    goPtr->move.set_MoveType( MoveType::Crawl );

    goPtr->set_collision_isDoPass( false );
    //--- 小树，中树 可以被其它go 穿过，成年树不行 ---
    if( pvtBp->age <= 2 ){
        goPtr->set_collision_isBePass( true );
    }else{
        goPtr->set_collision_isBePass( false );
    }
    
    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goPtr->creat_new_goMesh("root", //- gmesh-name
                                        "pineTree", //- animFrameSet-Name
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader,
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        gameObjs::apply_isFlipOver( _fieldWeight ) //- isFlipOver
                                        );

        //-- bind animFrameSet / animFrameIdxHandle --
        rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->oakId );

    //-- 务必在 mesh:"root" 之后 ---
    goPtr->goPos.set_alti( 0.0f );
    goPtr->goPos.init_by_currentMPos( _mpos );
    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( pineTree_pubBinaryValTypes );
}


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void PineTree::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void PineTree::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void PineTree::OnRenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );

    //=====================================//
    //           test: AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    //goPtr->move.RenderUpdate();
            // 目前来看，永远也不会 移动...


    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    for( auto &pairRef : goPtr->goMeshs ){
        pairRef.second.RenderUpdate();
    }
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void PineTree::OnLogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void PineTree::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

    cout << "PineTree::OnActionSwitch()"
        << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &rootGoMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->oakId );
                                    
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


namespace{//-------------- namespace ------------------//



/* ===========================================================
 *                     apply_a_oakId
 * -----------------------------------------------------------
 */
int apply_a_oakId( int _age, float _fieldWeight ){
    size_t  idx;
    int randV = gameObjs::apply_a_simpleId( _fieldWeight, 79 );

    switch( _age ){
        case 1: idx = randV % ids_age1.size();  return ids_age1.at(idx);
        case 2: idx = randV % ids_age2.size();  return ids_age2.at(idx);
        case 3: idx = randV % ids_age3.size();  return ids_age3.at(idx);
        case 4: idx = randV % ids_age4.size();  return ids_age4.at(idx);
        default:
            assert(0);
            return 0; //- never touch
    }
}



}//------------------ namespace: end ------------------//
}//------------- namespace gameObjs: end ----------------
