/*
 * ========================= BigMan.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/majorGos/BigMan.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

#include "debug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


namespace{//-------------- namespace ------------------//
}//------------------ namespace: end ------------------//


/* ===========================================================
 *                   init_in_autoMod
 * -----------------------------------------------------------
 */
void BigMan::init_in_autoMod(  GameObj *_goPtr,
                                const IntVec2 &_mpos,
					            float _fieldWeight,
					            const Altitude &_alti,
					            const Density &_density ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(BigMan_PvtBinary) );
    pvtBp = (BigMan_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &BigMan::OnRenderUpdate, &big_man, _1 );   
    goPtr->LogicUpdate  = std::bind( &BigMan::OnLogicUpdate,  &big_man, _1 );

    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &BigMan::OnActionSwitch, &big_man, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Move );


    //-------- go self vals ---------//
    goPtr->species = BigMan::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 5.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_3 );
    goPtr->move.set_MoveType( MoveType::Crawl );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goPtr->creat_new_goMesh("root", //- gmesh-name
                                        "bigMan", //- animFrameSet-Name
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader,
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        false //- isFlipOver
                                        );

        //-- bind animFrameSet / animFrameIdxHandle --
        rootGoMeshRef.getnc_animFrameIdxHandle().bind_cycle(0,   //- 起始图元帧序号
                                                5,   //- 结束图元帧序号
                                                0,   //- 入口图元帧序号  
                                                true //- isOrder
                                                );

    //-- 务必在 mesh:"root" 之后 ---
    goPtr->goPos.set_alti( 0.0f );
    goPtr->goPos.init_by_currentMPos( _mpos );
    
    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( bigMan_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void BigMan::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void BigMan::rebind( GameObj *_goPtr ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void BigMan::OnRenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();

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
void BigMan::OnLogicUpdate( GameObj *_goPtr ){
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
 * -- 
 */
void BigMan::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            //goMeshRef.bind_animFrameSet( "bigMan" );
            goMeshRef.getnc_animFrameIdxHandle().bind_cycle(0,   //- 起始图元帧序号
                                                    5,   //- 结束图元帧序号
                                                    0,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        case ActionSwitchType::Move_Move:
            //goMeshRef.bind_animFrameSet( "bigMan" );
            goMeshRef.getnc_animFrameIdxHandle().bind_cycle(6,   //- 起始图元帧序号
                                                    11,  //- 结束图元帧序号
                                                    6,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }


}


}//------------- namespace gameObjs: end ----------------
