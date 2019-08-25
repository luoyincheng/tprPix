/*
 * ========================= OneEyeBoy.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * 
 * ----------------------------
 */
#ifndef TPR_ONE_EYE_BOY_H
#define TPR_ONE_EYE_BOY_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "MapAltitude.h"
#include "Density.h"
#include "ParamBinary.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


struct OneEyeBoy_PvtBinary{
    int   tmp {};
};


class OneEyeBoy{
public:
    OneEyeBoy() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_,
                            const ParamBinary &dyParams_ );

private:

    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static  OneEyeBoy_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<OneEyeBoy_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
                            
    static void bind( GameObj &goRef_ );
    static void rebind( GameObj &goRef_ );

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnLogicUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 

