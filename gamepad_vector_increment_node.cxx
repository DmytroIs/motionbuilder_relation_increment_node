

//--- Class declaration
#include "gamepad_vector_increment_node.h"
#include <math.h>

//--- Registration defines
#define ORBOXVECTOR__CLASS		ORBOXVECTOR__CLASSNAME
#define ORBOXVECTOR__NAME		ORBOXVECTOR__CLASSSTR
#define	ORBOXVECTOR__LOCATION	"System"
#define ORBOXVECTOR__LABEL		"Gamepad Incrementation Node"
#define	ORBOXVECTOR__DESC		"Gamepad Incrementation Node"

//--- implementation and registration
FBBoxImplementation	(	ORBOXVECTOR__CLASS		);		// Box class name
FBRegisterBox		(	ORBOXVECTOR__NAME,				// Unique name to register box.
						ORBOXVECTOR__CLASS,				// Box class name
						ORBOXVECTOR__LOCATION,			// Box location ('plugins')
						ORBOXVECTOR__LABEL,				// Box label (name of box to display)
						ORBOXVECTOR__DESC,			// Box long description.
						FB_DEFAULT_SDK_ICON		);	// Icon filename (default=Open Reality icon)

/************************************************
 *	Creation
 ************************************************/
bool ORBoxVector::FBCreate()
{
	// Input Node
    mReset				= AnimationNodeInCreate  ( 0, "[1_0]  Reset", ANIMATIONNODE_TYPE_BOOL );
	mActive				= AnimationNodeInCreate  ( 0, "[1_1]  Active", ANIMATIONNODE_TYPE_BOOL );
	mThresholdXUp		= AnimationNodeInCreate  ( 0, "[3_0]  Deadzone threshold: X up limit     ", ANIMATIONNODE_TYPE_NUMBER );
	mThresholdXDown		= AnimationNodeInCreate  ( 0, "[3_1]  Deadzone threshold: X down limit", ANIMATIONNODE_TYPE_NUMBER );
	mThresholdYUp		= AnimationNodeInCreate  ( 0, "[3_2]  Deadzone threshold: Y up limit     ", ANIMATIONNODE_TYPE_NUMBER );
	mThresholdYDown		= AnimationNodeInCreate  ( 0, "[3_3]  Deadzone threshold: Y down limit", ANIMATIONNODE_TYPE_NUMBER );
	mThresholdZUp		= AnimationNodeInCreate  ( 0, "[3_4]  Deadzone threshold: Z up limit     ", ANIMATIONNODE_TYPE_NUMBER );
	mThresholdZDown		= AnimationNodeInCreate  ( 0, "[3_5]  Deadzone threshold: Z down limit", ANIMATIONNODE_TYPE_NUMBER );
	mInitialVector		= AnimationNodeInCreate  ( 0, "[4_0]  Starting value", ANIMATIONNODE_TYPE_VECTOR );
	mGamepadInput		= AnimationNodeInCreate  ( 0, "[0]     GAMEPAD INPUT", ANIMATIONNODE_TYPE_VECTOR );
	mGamepadValueOffset = AnimationNodeInCreate  ( 0, "[0_1]  Gamepad input offset", ANIMATIONNODE_TYPE_VECTOR );
	mIncrementSpeedX	= AnimationNodeInCreate  ( 0, "[2_0]  Increment speed X", ANIMATIONNODE_TYPE_NUMBER );
	mIncrementSpeedY	= AnimationNodeInCreate  ( 0, "[2_1]  Increment speed Y", ANIMATIONNODE_TYPE_NUMBER );
	mIncrementSpeedZ	= AnimationNodeInCreate  ( 0, "[2_2]  Increment speed Z", ANIMATIONNODE_TYPE_NUMBER );
	mFps				= AnimationNodeInCreate  ( 0, "[5_0]  FPS, 30 by default", ANIMATIONNODE_TYPE_NUMBER );
	

	// Output Nodes
    mVector				= AnimationNodeOutCreate ( 0, "OUT", ANIMATIONNODE_TYPE_VECTOR );

  // mY = AnimationNodeOutCreate ( 0, "Y", ANIMATIONNODE_TYPE_NUMBER );
  // mZ = AnimationNodeOutCreate ( 0, "Z", ANIMATIONNODE_TYPE_NUMBER );


	lReset=0; lActive=0; lThreshold=0; lIncrementSpeedX=0; lIncrementSpeedY=0; lIncrementSpeedZ=0; lFps=0;
	for (int i=0; i<3; i++) {lVector[i]=0; lInitialVector[i]=0; lGamepadInput[i]=0; lIncrementSpeedVector[i]=0, lGamepadValueOffset[i]=0;};	
	SysTimeInit = 0;
	lReset = 0 ;

	return true;
}


/************************************************
 *	Destruction.
 ************************************************/
void ORBoxVector::FBDestroy()
{
}


/************************************************
 *	Real-time engine evaluation
 ************************************************/
bool ORBoxVector::AnimationNodeNotify( FBAnimationNode* pAnimationNode, FBEvaluateInfo* pEvaluateInfo )
{
	bool	lStatus;	// Status of input node

	lStatus = mReset				->ReadData( &lReset, pEvaluateInfo );
	lStatus = mActive				->ReadData( &lActive, pEvaluateInfo ); 
	lStatus = mThresholdXUp			->ReadData( &lThresholdXUp, pEvaluateInfo );
	lStatus = mThresholdXDown		->ReadData( &lThresholdXDown, pEvaluateInfo ); 
	lStatus = mThresholdYUp			->ReadData( &lThresholdYUp, pEvaluateInfo ); 
	lStatus = mThresholdYDown		->ReadData( &lThresholdYDown, pEvaluateInfo ); 
	lStatus = mThresholdZUp			->ReadData( &lThresholdZUp, pEvaluateInfo ); 
	lStatus = mThresholdZDown		->ReadData( &lThresholdZDown, pEvaluateInfo ); 
	lStatus = mInitialVector		->ReadData(  lInitialVector, pEvaluateInfo ); 
	lStatus = mGamepadInput			->ReadData(  lGamepadInput, pEvaluateInfo ); 
	lStatus = mGamepadValueOffset	->ReadData(  lGamepadValueOffset, pEvaluateInfo ); 
	lStatus = mIncrementSpeedX		->ReadData( &lIncrementSpeedX, pEvaluateInfo ); 
	lStatus = mIncrementSpeedY		->ReadData( &lIncrementSpeedY, pEvaluateInfo ); 
	lStatus = mIncrementSpeedZ		->ReadData( &lIncrementSpeedZ, pEvaluateInfo ); 
	lStatus = mFps					->ReadData( &lFps, pEvaluateInfo ); 
	// Read the data from the input node
	//lStatus = mReset->ReadData( lVector, pEvaluateInfo );
	for (int i=0; i<3; i++) {lIncrementSpeedVector[0] = lIncrementSpeedX; lIncrementSpeedVector[1] = lIncrementSpeedY; lIncrementSpeedVector[2] = lIncrementSpeedZ;};
	lThresholdMatrix[0][0] = lThresholdXUp 		;
	lThresholdMatrix[0][1] = lThresholdXDown	;
	lThresholdMatrix[1][0] = lThresholdYUp 		;
	lThresholdMatrix[1][1] = lThresholdYDown	;
	lThresholdMatrix[2][0] = lThresholdZUp 		;
	lThresholdMatrix[2][1] = lThresholdZDown	;





	// If the read was not from a dead node.
    if( lStatus && lActive)
	{
		IncreametVectorLogic ();
		// Write the data to the out node
		//mX->WriteData( &lVector[0], pEvaluateInfo);
		//mY->WriteData( &lVector[1], pEvaluateInfo);
		mVector->WriteData( lVector, pEvaluateInfo);

	    return true;
	}

    return false;
}
//-----------------------------------------------------------------------------------------------------------
bool ORBoxVector::IncreametVectorLogic ()
{
	if (lReset) 
	{
		SysTimeInit = 0;
		for (int i=0;i<3;i++) {lVector[i] = lInitialVector[i];};
	}	
	if (!SysTimeInit) 
	{
		lSysTime0		= FBSystem().SystemTime;
		lSysTimeCurrent = FBSystem().SystemTime;
		SysTimeInit = 1;
	}

	lSysTimeCurrent = FBSystem().SystemTime;
	double TimeDelta =(lSysTimeCurrent.GetSecondDouble())-(lSysTime0.GetSecondDouble());
	if (lFps)
	{
		if (TimeDelta >= (1/lFps))					// trigger increament when with lFps per seconds times
		{
			lSysTime0 = FBSystem().SystemTime;		//reset deltatime fps threshold
			for (int i=0;i<3;i++) 
			{
				if ((lGamepadInput[i]-lGamepadValueOffset[i])>lThresholdMatrix[i][0] || (lGamepadInput[i]-lGamepadValueOffset[i])<lThresholdMatrix[i][1])
				{
					lVector[i] = lVector[i] + ((lGamepadInput[i]-lGamepadValueOffset[i])*0.01*lIncrementSpeedVector[i]);
				}
			}			
		}
	}

	//lVector [0] = TimeDelta;
	return true;
}
//-----------------------------------------------------------------------------------------------------------