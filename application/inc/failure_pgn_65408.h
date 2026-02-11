#ifndef __FAILURE_PGN_65408_H__
#define __FAILURE_PGN_65408_H__

#include <stdint.h>

enum 
{
    SPN_X3597 = 3597U,  /**< Напряжение внешнего питания 24V вне диапазона */ 
    SPN_X3598 = 3598U,  /**< Напряжение внутреннего питания 5.0V вне диапазона */
    SPN_X3599 = 3599U,  /**< Напряжение внутреннего питания 3.3V вне диапазона */
    SPN_X0152 = 152U,   /**< Сброс платы и следовательно автоматическое восстановление функционирования */
    SPN_X2802 = 2802U,  /**< Тест Ram ошибка */                                  
};
enum 
{
    FMI_X0 = 0U,      /**< Data Valid but Above Normal Operational Range              */
    FMI_X1 = 1U,      /**< Data Valid but Below Normal Operational Range              */
    FMI_X2 = 2U,      /**< Data Erratic, Intermittent or Incorrect                    */
    FMI_X3 = 3U,      /**< Voltage Above Normal, or Shorted to High Source            */
    FMI_X4 = 4U,      /**< Voltage Below Normal, or Shorted to High Source            */
    FMI_X5 = 5U,      /**< Current Below Normal, or Open Circuit                      */
    FMI_X6 = 6U,      /**< Current Above Normal, or Grounded Circuit                  */
    FMI_X7 = 7U,      /**< Mechanical System not Responding or Out of Adjustment      */
    FMI_X8 = 8U,      /**< Abnormal Frequency or Pulse Width or Period                */
    FMI_X9 = 9U,      /**< Abnormal Update Rate                                       */
    FMI_X10 = 10U,    /**< Abnormal Rate of Change                                    */
    FMI_X11 = 11U,    /**< Failure Code not Identifiable                              */
    FMI_X12 = 12U,    /**< Bad Intelligent Device or Component                        */
    FMI_X13 = 13U,    /**< Out of Calibration                                         */
    FMI_X14 = 14U,    /**< Special Instructions                                       */
    FMI_X15 = 15U,    /**< Data Valid but Above Normal Range: Least Severe Level      */
    FMI_X16 = 16U,    /**< Data Valid but Above Normal Range: Moderately Severe Level */
    FMI_X17 = 17U,    /**< Data Valid but Below Normal Range: Least Severe Level      */
    FMI_X18 = 18U,    /**< Data Valid but Below Normal Range: Moderately Severe Level */
    FMI_X19 = 19U,    /**< Received Network Data in Error: (Multiplexed Data)         */
    FMI_X20 = 20U,    /**< Data Drifted High (rationality high)                       */
    FMI_X21 = 21U,    /**< Data Drifted Low (rationality low)                         */
    FMI_X31 = 31U     /**< Condition Exists                                           */
};

typedef struct failure_pgn_65408_s
{ 
        const uint16_t pgn;
        int (*init)(void);   
        int (*set)(int spn,int fmi);
        int (*reset)(int spn);
        int (*send_answer)(void);  
}failure_pgn_65408_t;

extern const failure_pgn_65408_t failure_pgn_65408;

#endif /* __FAILURE_PGN_65408_H__ */
