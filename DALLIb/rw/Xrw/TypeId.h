#ifndef __RW_TYPE_ID_H
#define __RW_TYPE_ID_H
/*********************************************************************
*
*  $Id: TypeId.h,v 1.9 1993/10/22 22:05:14 keffer Exp $
*
*  COPYRIGHT   : (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*                The software and information contained herein
*                are proprietary to, and comprise valuable trade
*                secrets of, Rogue Wave Software, Inc., which
*                intends to preserve as trade secrets such software
*                and information.  This software is furnished pursuant
*                to a written license agreement and may be used,
*                copied, transmitted, and stored only in accordance
*                with the terms of such license and with the inclusion
*                of the above copyright notice.  This software and
*                information or any other copies thereof may not be
*                provided or otherwise made available to any other
*                person.
*  
*  NOTICE      : Notwithstanding any other lease or license that may
*                pertain to, or accompany the delivery of, this
*                computer software and information, the rights of the
*                Government regarding its use, reproduction and
*                disclosure are as set forth in Section 52.227-19 of
*                the FARS Computer Software-Restricted Rights clause.
*  
*  RESTRICTED 
*  RIGHTS 
*  NOTICE      : Use, duplication, or disclosure by the Government
*                is subject to restrictions as set forth in
*                subparagraph (c)(1)(ii) of the Rights in Technical
*                Data and Computer Software clause at DFARS
*                52.227-7013.
*  
*  RESTRICTED
*  RIGHTS
*  LEGEND      : This computer software and information is distributed
*                with "restricted rights."  Use, duplication or
*                disclosure is subject to restrictions as set forth
*                in NASA FAR SUP 18-52.227-79 (April 1985)
*                "Commercial Computer Software-Restricted Rights
*                (April 1985)."  If the Clause at 18-52.227-74
*                "Rights in Data General" is specified in the
*                contract, then the "Alternate III" clause applies.
*
*                (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*  TRADEMARKS  : Rogue Wave is a trademark of
*                   Rogue Wave Software, Inc.
*                OSF/Motif is a trademark of
*                   Open Software Foundation, Inc.
*                Motif is a trademark of
*                   Open Software Foundation, Inc.
*                X Window System is a trademark of
*                   the Massachusetts Institute of Technology
*
*  Rogue Wave  : Rogue Wave Software, Inc.
*                P.O. Box 2328
*                Corvallis, OR 97330
*                (503) 754-2311
*
*  ------------------------------------------------------------------
*
*  File        : TypeId.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the definition of all RW Tools 
*                Class IDs.  
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#ifndef __RWVIEWDEFS_H__
#  include "rw/Xrw/viewdefs.h"
#endif

#include "rw/Xrw/TypeIdUbs.h"


#define __RWAPPLICATION                    0x7000
#define __RWCONTROLLER                     0x7001
#define __RWANYCALLBACK                    0x7002
#define __RWAPPLICATIONSHELL               0x7003
#define __RWARROWBUTTON                    0x7004
#define __RWARROWBUTTONGADGET              0x7005
#define __RWARROWBUTTONCALLBACK            0x7006
#define __RWBULLETINBOARD                  0x7007
#define __RWBULLETINBOARDDIALOG            0x7008
#define __RWCALLBACK                       0x7009
#define __RWCALLBACKCB                     0x700a
#define __RWCASCADEBUTTON                  0x700b
#define __RWCASCADEBUTTONGADGET            0x700c
/*#define __RWCLIPBOARD                      0x700d*/
#define __RWCOMMANDCALLBACK                0x700e
#define __RWCOMMAND                        0x700f
#define __RWCOMPOSITE                      0x7010
#define __RWCORE                           0x7011
#define __RWDIALOGSHELL                    0x7012
#define __RWDRAWINGAREACALLBACK            0x7013
#define __RWDRAWINGAREA                    0x7014
#define __RWDRAWNBUTTON                    0x7015
#define __RWDRAWNBUTTONCALLBACK            0x7016
#define __RWERRORMESS                      0x70070
#define __RWFILESELECTIONBOX               0x7018
#define __RWFILESELECTIONDIALOG            0x7019
#define __RWFILESELECTIONBOXCALLBACK       0x701a
#define __RWFONTLIST                       0x701b
#define __RWFORM                           0x701c
#define __RWFORMDIALOG                     0x701d
#define __RWFRAME                          0x701e
#define __RWGADGET                         0x701f
#define __RWLABEL                          0x7020
#define __RWLABELGADGET                    0x7021
#define __RWLIST                           0x7022
#define __RWSCROLLEDLIST                   0x7023
#define __RWLISTCALLBACK                   0x7024
#define __RWMFCALLBACK                     0x7025
#define __RWMAINWINDOW                     0x7026
#define __RWMANAGER                        0x70270
#define __RWMENUSHELL                      0x7028
#define __RWMESSAGEBOX                     0x7029
#define __RWMESSAGEDIALOG                  0x702a
#define __RWINFORMATIONDIALOG              0x702b
#define __RWQUESTIONDIALOG                 0x702c
#define __RWWARNINGDIALOG                  0x702d
#define __RWERRORDIALOG                    0x702e
#define __RWWORKINGDIALOG                  0x702f
#define __RWMETHODCB                       0x7030
#define __RWOVERRIDESHELL                  0x7031
#define __RWPANEDWINDOW                    0x7032
#define __RWPRIMITIVE                      0x7033
#define __RWPUSHBUTTON                     0x7034
#define __RWPUSHBUTTONGADGET               0x7035
#define __RWPUSHBUTTONCALLBACK             0x7066
#define __RWROWCOLUMNCALLBACK              0x7036
#define __RWROWCOLUMN                      0x70370
#define __RWMENUBAR                        0x7038
#define __RWOPTIONMENU                     0x7039
#define __RWPOPUPMENU                      0x703a
#define __RWPULLDOWNMENU                   0x703b
#define __RWRADIOBOX                       0x703c
#define __RWSCALE                          0x703d
#define __RWSCALECALLBACK                  0x703e
#define __RWSCROLLBARCALLBACK              0x703f
#define __RWSCROLLBAR                      0x7040
#define __RWSCROLLEDWINDOW                 0x7041
#define __RWSELECTIONBOXCALLBACK           0x7042
#define __RWSELECTIONBOX                   0x7043
#define __RWPROMPTDIALOG                   0x7044
#define __RWSELECTIONDIALOG                0x7045
#define __RWSEPARATORGADGET                0x7046
#define __RWSEPARATOR                      0x7047
#define __RWSHELL                          0x7048
#define __RWXMSTRING                       0x7049
#define __RWXMSTRINGDIRECTION              0x704a
#define __RWXMSTRINGSEGMENT                0x704b
#define __RWXMSTRINGSEPARATOR              0x704c
#define __RWTEXT                           0x704d
#define __RWSCROLLEDTEXT                   0x704e
#define __RWTEXTVERIFYCALLBACK             0x704f
#define __RWTOGGLEBUTTON                   0x7050
#define __RWTOGGLEBUTTONCALLBACK           0x7051
#define __RWTOGGLEBUTTONGADGET             0x7052
#define __RWTOPLEVELSHELL                  0x7053
#define __RWTRANSIENTSHELL                 0x7054
#define __RWUSERDATA                       0x7055
#define __RWVENDORSHELL                    0x7056
#define __RWCALLBACKINFO                   0x7057
#define __RWEVENTCB                        0x7058
#define __RWEVENTMETHODCB                  0x7059
#define __RWTEXTFIELD                      0x705c

#if RWX11Version>=5
#define __RWFONTLISTENTRY                  0x705d
#define __RWXMSCREEN                       0x705e
#define __RWXMDISPLAY                      0x705f
#define __RWDRAGICON                       0x7060
#define __RWDRAGCONTEXT			   0x7061
#define __RWDROPSITE                       0x7062
#define __RWDROPTRANSFER                   0x7063
#define __RWTEMPLATEDIALOG                 0x7064
#endif

#define __RWSEARCHPROCCB                   0x7065

/* View Hierarchy classIDs */


#define __RWACCELERATOR               0x7100
#define __RWMENU                      0x7101
#define __RWMENUARG                   0x7102
#define __RWMENUOBJECT                0x7103
#define __RWTOPMENU                   0x7104
#define __RWVIEW                      0x7105
#define __RWMAINVIEW                  0x7106
#define __RWAUXVIEW                   0x7107
#define __RWDIALOG                    0x7108
#define __RWBUTTONDIALOG              0x7109
#define __RWCONFIRMDIALOG             0x710a
#define __RWCUSTOMDIALOG              0x710b
#define __RWFILEDIALOG                0x710c
#define __RWLISTDIALOG                0x710d
#define __RWNOTIFYDIALOG              0x710e
#define __RWREQUESTDIALOG             0x710f
#define __RWABOUTDIALOG               0x7110
#define __RWMENUITEM                  0x7111
#define __RWTOPVIEW                   0x7112

/*Model-level object definitions*/
#define __RWUSERMODEL                 0x7200
#define __RWASSOCIATION               0x7201
#define __RWMOUSENOTIFIER             0x7201
#endif   
