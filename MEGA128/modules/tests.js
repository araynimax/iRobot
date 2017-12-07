var   MAX = 224;
var   MIN = 125;
var   MID = 181;
var RANGE = 180;
var  GRAD =   90;

var halfAngleRange = RANGE / 2;
var leftRange    = MID - MIN;
var rightRange   = MAX - MID;

var leftMulti    = leftRange / halfAngleRange;
var rightMulti   = rightRange / halfAngleRange;

var leftValue  =  MIN + leftRange - leftMulti * GRAD;
var rightValue =  MID + leftMulti * GRAD;


var TIMER_VALUE = (GRAD < 0 ? (MIN + (MID - MIN) - ((MID - MIN) / (RANGE / 2)) * GRAD * (-1)) : (MID + ((MAX - MID) / (RANGE / 2)) * GRAD));

console.log(TIMER_VALUE);
