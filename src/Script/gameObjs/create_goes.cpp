/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/create_goes.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/resource/ssrc.h"

#include "Script/json/GoJsonData.h"

#include "tprDebug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------



/* ===========================================================
 *                  create_a_Go       [tmp]
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Go( goSpecId_t goSpecId_,
                    const IntVec2 mpos_, 
                    const IntVec2 pposOff_,
                    const ParamBinary &dyParams_ ){

    goid_t goid = esrc::insert_new_gameObj( mpos_, pposOff_ );
    GameObj &goRef = esrc::get_goRef( goid );

        tprAssert( ssrc::find_from_goInit_funcs(goSpecId_) );

    //-- set some static datas from JSON --
    assemble_goJsonData_2_newGo( goSpecId_, goRef );

    // 用 mpos, pposoff 合成 dpos 

    ssrc::call_goInit_func( goSpecId_,
                            goRef,
                            dyParams_ );

    //------------------------------//
    goRef.signUp_newGO_to_mapEnt();
    esrc::insert_2_goids_inactive( goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...

    
    return  goid;
}


/* ===========================================================
 *                  rebind_a_disk_Go       [tmp]
 * -----------------------------------------------------------
 * 从 db读取一个 go 的数据，并用此数据，重建一个 mem态 go实例
 */
void rebind_a_disk_Go(  const DiskGameObj &diskGo_,
                        const IntVec2 mpos_, 
                        const IntVec2 pposOff_,
                        const ParamBinary &dyParams_  ){

    esrc::insert_a_disk_gameObj( diskGo_.goid, mpos_, pposOff_ );
    GameObj &goRef = esrc::get_goRef( diskGo_.goid );

        tprAssert( ssrc::find_from_goInit_funcs(diskGo_.goSpecId) );

    //-- set some static datas from JSON --
    assemble_goJsonData_2_newGo( diskGo_.goSpecId, goRef ); //- tmp
                    //-- 临时措施
                    //   在未来，已经组装 从 数据库取出的数据，而不是从 json 中
                    //   至少有一部分吧
                    //   ...

    ssrc::call_goInit_func( diskGo_.goSpecId,
                            goRef,
                            dyParams_ );

            //-- 临时方案，最好使用 具象go类 rebind 系列函数 
            
    //------------------------------//
    goRef.signUp_newGO_to_mapEnt();
    esrc::insert_2_goids_inactive( diskGo_.goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
}


}//------------- namespace gameObjs: end ----------------

