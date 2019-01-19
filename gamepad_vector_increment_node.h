#ifndef __ORBOX_VECTOR_BOX_H__
#define __ORBOX_VECTOR_BOX_H__


//--- SDK include
#include <fbsdk/fbsdk.h>
#include <iostream>
#include <string>

//--- Registration defines
#define	ORBOXVECTOR__CLASSNAME		ORBoxVector
#define ORBOXVECTOR__CLASSSTR		"ORBoxVector2"


class ORBoxVector : public FBBox
{
	//--- box declaration.
	FBBoxDeclare( ORBoxVector, FBBox );

public:
	virtual bool FBCreate();		//!< creation function.
	virtual void FBDestroy();		//!< destruction function.

	//! Overloaded FBBox real-time evaluation function.
	virtual bool AnimationNodeNotify(FBAnimationNode* pAnimationNode,FBEvaluateInfo* pEvaluateInfo);

	bool IncreametVectorLogic ();

private:
	FBAnimationNode* 	mReset, *mActive, *mInitialVector, *mGamepadInput, *mIncrementSpeedX, *mIncrementSpeedY, *mIncrementSpeedZ, *mFps, *mThresholdXUp, *mThresholdXDown, *mThresholdYUp, *mThresholdYDown, *mThresholdZUp, *mThresholdZDown, *mGamepadValueOffset;			//!< Input node:	Vector
	FBAnimationNode*	mVector;	//!< Output nodes:	X,Y,Z
	double lReset, lActive, lThreshold, lInitialVector[3], lGamepadInput[3], lIncrementSpeedX, lIncrementSpeedY, lIncrementSpeedZ, lFps, lThresholdXUp, lThresholdXDown, lThresholdYUp, lThresholdYDown, lThresholdZUp, lThresholdZDown, lGamepadValueOffset[3]; 
	double lVector[3], lIncrementSpeedVector[3], lThresholdMatrix[3][2];
	FBTime lSysTime0, lSysTimeCurrent;
	bool SysTimeInit;
};

#endif /* __ORBOX_VECTOR_BOX_H__ */
