#ifndef IncludedMainInclude
#define IncludedMainInclude
#include "MainInclude.h"
#endif



Vtx V64x32[] ={
    {   {  {-32,     0,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,     0,  0}, 0,  {(( 64-1)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    32,  0}, 0,  {(( 64-1)<<6),    (( 32-1)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-32,    32,  0}, 0,  {0,               (( 32-1)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
};


Vtx V1632[] ={
    {   {  {-8,     0,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 8,     0,  0}, 0,  {(( 16-1)<<6),     0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 8,    32,  0}, 0,  {(( 16-1)<<6),     (( 32-1)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-8,    32,  0}, 0,  {0,               (( 32-1)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
};


Vtx V6432B[] ={
    {   {  {-32,     -32,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,     -32,  0}, 0,  {(( 64)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    -16,  0}, 0,  {(( 64)<<6),    (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-32,    -16,  0}, 0,  {0,               (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

    {   {  {-32,    -16,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    -16,  0}, 0,  {(( 64)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    0,  0}, 0,  {(( 64)<<6),    (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-32,    0,  0}, 0,  {0,               (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

    {   {  {-32,    0,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    0,  0}, 0,  {(( 64)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    16,  0}, 0,  {(( 64)<<6),    (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-32,    16,  0}, 0,  {0,               (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

    {   {  {-32,    16,  0}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    16,  0}, 0,  {(( 64)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 32,    32,  0}, 0,  {(( 64)<<6),    (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-32,    32,  0}, 0,  {0,               (( 16)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
};


Vtx V256x12832B[] ={
	{   {  {-128,     -64,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,     -64,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -60,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -60,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -60,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -60,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -56,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -56,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -56,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -56,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -52,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -52,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -52,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -52,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   -48,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   -48,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	//

	{   {  {-128,     -48,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,     -48,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,    -44,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    	{   {  {-128,    -44,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -44,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -44,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -40,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -40,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -40,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -40,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -36,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -36,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -36,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -36,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   -32,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   -32,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	//

	{   {  {-128,     -32,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,     -32,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -28,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -28,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -28,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -28,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   -24,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -24,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -24,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -24,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -20,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -20,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -20,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -20,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   -16,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   -16,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	//

	{   {  {-128,     -16,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,     -16,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,    -12,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    	{   {  {-128,    -12,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -12,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -12,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -8,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -8,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -8,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -8,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -4,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    -4,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    -4,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    -4,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   0,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   0,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

     {   {  {-128,     0,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,     0,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    4,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    4,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    4,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    4,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    8,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    8,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    8,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    8,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    12,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    12,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    12,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    12,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   16,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   16,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	//

	{   {  {-128,     16,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,     16,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,    20,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    	{   {  {-128,    20,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    20,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    20,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    24,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    24,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    24,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    24,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    28,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    28,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    28,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    28,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   32,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   32,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	//

	{   {  {-128,     32,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,     32,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    36,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    36,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    36,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    36,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    40,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    40,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    40,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    40,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    44,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    44,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    44,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    44,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   48,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   48,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	//

	{   {  {-128,     48,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,     48,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
    	{   {  { 128,    52,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
    	{   {  {-128,    52,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    52,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    52,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    56,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    56,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    56,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    56,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    60,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,    60,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

	{   {  {-128,    60,  -20}, 0,  {0,               0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,    60,  -20}, 0,  {(( 256)<<6),    0},              {0xff, 0xff, 0xff, 0xff} } },
	{   {  { 128,   64,  -20}, 0,  {(( 256)<<6),    (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },
	{   {  {-128,   64,  -20}, 0,  {0,               (( 4)<<6)},   {0xff, 0xff, 0xff, 0xff} } },

};

unsigned short TreePalette[]={
0x0000,0x0041,0x0081,0x00c1,0x0101,0x0141,0x0181,
0x01c1,0x0201,0x0241,0x0281,0x02c1,0x0301,0x0341,
0x0381,0x03c1,0x0401,0x0441,0x0481,0x04c1,0x0501,
0x0541,0x08c1,0x0901,0x0941,0x0981,0x09c1,0x0a01,
0x0a81,0x0ac1,0x0b01,0x0b41,0x0b81,0x0bc1,0x0c01,
0x0c41,0x0c81,0x0cc1,0x0d01,0x0d41,0x0d81,0x0dc1,
0x0e01,0x0e41,0x1101,0x1141,0x1181,0x1183,0x1185,
0x11c1,0x11c3,0x11c5,0x1201,0x1203,0x1205,0x1241,
0x1243,0x1281,0x12c1,0x1301,0x1341,0x13c1,0x14c3,
0x1501,0x1541,0x1543,0x1581,0x15c1,0x1601,0x1641,
0x1681,0x16c1,0x1701,0x1741,0x1781,0x17c1,0x1941,
0x1981,0x19c1,0x1a01,0x1a05,0x1a41,0x1a45,0x1a81,
0x1a83,0x1a85,0x1ac3,0x1ac5,0x1b05,0x1b45,0x1b83,
0x1b85,0x1bc5,0x1c03,0x1c05,0x1c45,0x1c85,0x1cc5,
0x1dc5,0x1e43,0x1fc1,0x2083,0x20c3,0x2103,0x2183,
0x2201,0x2241,0x2281,0x2283,0x22c1,0x2301,0x2487,
0x24c5,0x2505,0x2545,0x2547,0x2585,0x2587,0x25c5,
0x2605,0x2685,0x2983,0x29c3,0x2a81,0x2ac1,0x2b01,
0x2b41,0x2cc7,0x2d87,0x2dc7,0x2e07,0x2e47,0x30c5,
0x3105,0x31c3,0x32c1,0x3341,0x3381,0x3383,0x33c1,
0x3401,0x360b,0x3649,0x3bc1,0x3c01,0x3c41,0x3e8b,
0x3ecb,0x3f4b,0x4105,0x4145,0x4407,0x4441,0x4481,
0x44c1,0x4501,0x4503,0x4545,0x468f,0x4947,0x4987,
0x4c0b,0x4d01,0x4d03,0x4d41,0x4dcb,0x5187,0x51c9,
0x5307,0x5501,0x5541,0x5543,0x5581,0x5711,0x5753,
0x59c9,0x5dc1,0x5dc3,0x5e01,0x5e03,0x624b,0x6589,
0x65c1,0x6601,0x6609,0x6641,0x6681,0x6e41,0x6e43,
0x6ec1,0x6f09,0x6f97,0x728d,0x7551,0x7641,0x76c3,
0x7701,0x7a8d,0x7ec1,0x7f01,0x7f05,0x7f41,0x7f81,
0x7fd7,0x82cf,0x830d,0x8651,0x8781,0x87c1,0x87dd,
0x8b8f,0x8fc1,0x8fc5,0x9451,0x9703,0x97c1,0x9f83,
0x9fc1,0x9fdb,0xafdd,0xb7c1,0xb7c9,0xb7e9,0xbfc1,
0xcfc1,0xd7c5,0xd7db,0xe7c9,0xffd3,0xffd9,0x0000};

Vtx Tree1V[4]={
	{{ {40,10,0},	0, {1536, 1706},  	{255, 254, 254, 255} } },
	{{ {0,100,0},	0, {512, -341},  	{255, 254, 254, 255} } },
	{{ {-40,10,0},	0, {-512, 1706},  	{255, 254, 254, 255} } },
	{{ {0,-10,0},	0, {512, 2161},  	{255, 254, 254, 255} } }
};

Vtx PakkunV[]={
	{{ {-15,30,0}, 0, 	{   0, 	   0},  {255, 255, 254, 255} }},
	{{ {-15, 0,0}, 0, 	{   0, 	2048},  {255, 255, 254, 255} }},
	{{ { 15, 0,0}, 0, 	{2048, 	2048},  {255, 255, 254, 255} }},
	{{ { 15,30,0}, 0, 	{2048, 	   0},  {255, 255, 254, 255} }}
};
unsigned short PakkunColor[]={
0x0000,0x0101,0x0141,0x0181,0x01c1,0x0901,0x09c1,
0x0a01,0x0a41,0x0ac1,0x1201,0x1343,0x1981,0x1b85,
0x1d83,0x2385,0x23c7,0x2543,0x2901,0x2b03,0x2bc7,
0x2c45,0x2d05,0x2d45,0x2d47,0x3101,0x3183,0x31c3,
0x3243,0x3589,0x3801,0x38c1,0x3e0b,0x3e8b,0x4001,
0x4041,0x4043,0x40c3,0x4101,0x4149,0x4485,0x46cb,
0x46cd,0x470d,0x4801,0x4843,0x4885,0x4907,0x4983,
0x498d,0x49cd,0x4a03,0x4a0f,0x4e0d,0x4e8d,0x4ecd,
0x4f0d,0x4f0f,0x4f4f,0x5001,0x5043,0x5085,0x50c7,
0x5109,0x514b,0x51cf,0x5211,0x5241,0x56cf,0x574f,
0x5751,0x5801,0x5843,0x5883,0x58c1,0x59cf,0x5a11,
0x5a53,0x5a95,0x5ac1,0x5ad7,0x5f53,0x5f93,0x6001,
0x6043,0x60c1,0x6145,0x6183,0x6211,0x6241,0x6253,
0x6295,0x62c1,0x6301,0x6793,0x6801,0x6843,0x6881,
0x6883,0x68c3,0x698d,0x69c1,0x6a53,0x6ad7,0x6b01,
0x6b41,0x7001,0x7043,0x7083,0x7319,0x735b,0x7381,
0x739d,0x7801,0x7a53,0x8001,0x80c7,0x8109,0x8149,
0x83c1,0x8401,0x8421,0x8801,0x8843,0x8885,0x8a11,
0x8ad7,0x8b05,0x8b9d,0x8c63,0x9001,0x9405,0x9801,
0x9843,0x9a47,0x9b19,0x9c81,0x9ca5,0xa001,0xa0c7,
0xa1c5,0xa463,0xa4e7,0xa501,0xa801,0xa843,0xa885,
0xa94b,0xaa0f,0xaa95,0xacc1,0xb001,0xb463,0xb56b,
0xb5ad,0xb801,0xb843,0xb885,0xb8c5,0xb98d,0xbd81,
0xbdc1,0xbdef,0xc001,0xc081,0xc0c5,0xc251,0xc35b,
0xc801,0xcad5,0xcdc1,0xce01,0xce31,0xd001,0xd083,
0xd5ad,0xd681,0xd6b5,0xd801,0xdce7,0xddef,0xde73,
0xdef7,0xe001,0xe043,0xe085,0xe18d,0xe211,0xe319,
0xe35b,0xe39d,0xe463,0xe569,0xe739,0xe801,0xe841,
0xe8c7,0xe909,0xe9cf,0xea95,0xec21,0xee31,0xef41,
0xef4b,0xef7b,0xf001,0xf085,0xf14b,0xf463,0xf781,
0xf785,0xf7bd,0xf801,0xf843,0xf885,0xf8c7,0xf909,
0xf94b,0xf98d,0xf9cf,0xfa11,0xfa53,0xfa95,0xfad7,
0xfb19,0xfb5b,0xfb9d,0xfbdf,0xfc21,0xfc63,0xfca5,
0xfce7,0xfd29,0xfd6b,0xfdad,0xfdef,0xfe31,0xfe73,
0xfeb5,0xfef7,0xff39,0xff41,0xff7b,0xff81,0xffbd,
0xffc1,0xffc5,0xffc7,0xffcd,0xffff,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000
};


///


void ScaleMatrixXYZFixed(AffineMtx Matrix, SVector FScale)
{
    Matrix[0][0] *= (float)(FScale[0] * 0.01f);
    Matrix[0][1] *= (float)(FScale[0] * 0.01f);
    Matrix[0][2] *= (float)(FScale[0] * 0.01f);
    Matrix[1][0] *= (float)(FScale[1] * 0.01f);
    Matrix[1][1] *= (float)(FScale[1] * 0.01f);
    Matrix[1][2] *= (float)(FScale[1] * 0.01f);
    Matrix[2][0] *= (float)(FScale[2] * 0.01f);
    Matrix[2][1] *= (float)(FScale[2] * 0.01f);
    Matrix[2][2] *= (float)(FScale[2] * 0.01f);
}
void ScaleMatrixXYZ(AffineMtx Matrix, Vector Scale)
{
    Matrix[0][0] *= Scale[0];
    Matrix[0][1] *= Scale[0];
    Matrix[0][2] *= Scale[0];
    Matrix[1][0] *= Scale[1];
    Matrix[1][1] *= Scale[1];
    Matrix[1][2] *= Scale[1];
    Matrix[2][0] *= Scale[2];
    Matrix[2][1] *= Scale[2];
    Matrix[2][2] *= Scale[2];
}



/*

typedef struct {
	short		ob[3];	/// x, y, z 
	unsigned short	Flag;
	short		tc[2];	/// Texture coord 
	unsigned char	cn[4];	/// color & Alpha
} Vtx_t;

*/


/*
((sin(x+y+Time))/(intensity))+((sin(-x+y+Time))/(intensity))
*/
short WaveTime, WaveDirection = 1;

void WaveRace(Vtx_t *VertexBuffer, int Count, float Intensity)
{	
	
	{
	    int ThisVert;
	for (ThisVert = 0; ThisVert < Count; ThisVert++)
	{
		GlobalFloatA = SinTable[(Ushort)( (Uint)( 10000.0 * (VertexBuffer[ThisVert].ob[0] + VertexBuffer[ThisVert].ob[2]) + (WaveTime * 500) ) % 0xFFFF) >> 4];
		
		VertexBuffer[ThisVert].ob[1] = GlobalFloatA * Intensity ;
		
		VertexBuffer[ThisVert].cn[0] = (char)(115 + (50 * GlobalFloatA));
		VertexBuffer[ThisVert].cn[1] = (char)(115 + (50 * GlobalFloatA));
		VertexBuffer[ThisVert].cn[2] = (char)(165 + (60 * GlobalFloatA));
	}
	}
}

void DrawStereoscopic3D(short FocusValue)
{
	AspectRatio = (float)(240 / 160);
	
	GlobalScreen[0]->Width = 160;
	GlobalScreen[0]->Height = 240;
	GlobalScreen[0]->Posx = 80;
	GlobalScreen[0]->Posy = 120;
	KWLap[0].cx = 80;
	KWLap[0].cx2 = 80;
	KWLap[0].cx3 = 80;
	KWLap[0].timerX = 80;
	KWLap[0].Lap1CompletionTimeX = 80;
	KWLap[0].Lap2CompletionTimeX = 80;
	KWLap[0].Lap3CompletionTimeX = 80;
	KWLap[0].totalTimeX = 80;
	KWLap[0].ty = 20;
	KWLap[0].cy = 40;
	KWLap[0].ix = 40 - KWLap[0].Addix;
	KWLap[0].iy = 55;
	KWLap[0].rgx = 0;
	KWLap[0].rx = 40;
	KWLap[0].ry = 0;
	KWLap[0].rgy = 200;
	KWLap[0].Ranksw = 1;

	KWRank[1] = KWRank[0];
	

	GlobalScreen[1]->Width = 160;
	GlobalScreen[1]->Height = 240;
	GlobalScreen[1]->Posx = 240;
	GlobalScreen[1]->Posy = 120;
	KWLap[1].cx = 240;
	KWLap[1].cx2 = 240;
	KWLap[1].cx3 = 240;
	KWLap[1].timerX = 240;
	KWLap[1].Lap1CompletionTimeX = 240;
	KWLap[1].Lap2CompletionTimeX = 240;
	KWLap[1].Lap3CompletionTimeX = 240;
	KWLap[1].totalTimeX = 240;
	KWLap[1].ty = 20;
	KWLap[1].cy = 40;
	KWLap[1].ix = 200 - KWLap[1].Addix;
	KWLap[1].iy = 55;
	KWLap[1].rgx = 0;
	KWLap[1].rx = 200;
	KWLap[1].ry = 0;
	KWLap[1].rgy = 200;
	KWLap[1].Ranksw = 1;
	
	

	GlobalPlayer[1].Position[0] = GlobalPlayer[0].Position[0];
	GlobalPlayer[1].Position[1] = GlobalPlayer[0].Position[1] + 20;
	GlobalPlayer[1].Position[2] = GlobalPlayer[0].Position[2];
	
	GlobalPlayer[1].Direction[1] = GlobalPlayer[0].Direction[1];
	GlobalPlayer[1].Radius = 0.0001;
	GlobalPlayer[1].Offsetsize = 0.0001;
	/*
	GlobalPlayer[1].Bump.BumpXY[0] = GlobalPlayer[0].Bump.BumpXY[0];
	GlobalPlayer[1].Bump.BumpXY[1] = GlobalPlayer[0].Bump.BumpXY[1];
	GlobalPlayer[1].Bump.BumpXY[2] = GlobalPlayer[0].Bump.BumpXY[2];
	GlobalPlayer[1].Bump.BumpYZ[0] = GlobalPlayer[0].Bump.BumpYZ[0];
	GlobalPlayer[1].Bump.BumpYZ[1] = GlobalPlayer[0].Bump.BumpYZ[1];
	GlobalPlayer[1].Bump.BumpYZ[2] = GlobalPlayer[0].Bump.BumpYZ[2];
	GlobalPlayer[1].Bump.BumpZX[0] = GlobalPlayer[0].Bump.BumpZX[0];
	GlobalPlayer[1].Bump.BumpZX[1] = GlobalPlayer[0].Bump.BumpZX[1];
	GlobalPlayer[1].Bump.BumpZX[2] = GlobalPlayer[0].Bump.BumpZX[2];
	
	GlobalPlayer[1].Bump.LastXY = GlobalPlayer[0].Bump.LastXY;
	GlobalPlayer[1].Bump.LastYZ = GlobalPlayer[0].Bump.LastYZ;
	GlobalPlayer[1].Bump.LastZX = GlobalPlayer[0].Bump.LastZX;
	GlobalPlayer[1].Bump.FlagXY = GlobalPlayer[0].Bump.FlagXY;
	GlobalPlayer[1].Bump.FlagYZ = GlobalPlayer[0].Bump.FlagYZ;
	
	GlobalPlayer[1].Bump.FlagZX = GlobalPlayer[0].Bump.FlagZX;
	*/
	g_zoomFOVPlayer2 = g_zoomFOVPlayer1;
	g_zoomLevelPlayer2 = g_zoomLevelPlayer1;

	ObjectVelocity[0] = (float)(FocusValue * -0.1);
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = -5;
	MakeAlignVector(ObjectVelocity,GlobalPlayer[0].Direction[1]);

	GlobalCamera[0]->CameraPos[0] +=ObjectVelocity[0];
	GlobalCamera[0]->CameraPos[1] +=ObjectVelocity[1];
	GlobalCamera[0]->CameraPos[2] +=ObjectVelocity[2];

	
	ObjectVelocity[0] = (float)(FocusValue * 0.2);	
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 0;
	MakeAlignVector(ObjectVelocity,GlobalPlayer[0].Direction[1]);

	GlobalCamera[1]->CameraPos[0] = GlobalCamera[0]->CameraPos[0] + (ObjectVelocity[0]);
	GlobalCamera[1]->CameraPos[1] = GlobalCamera[0]->CameraPos[1] + (ObjectVelocity[1]);
	GlobalCamera[1]->CameraPos[2] = GlobalCamera[0]->CameraPos[2] + (ObjectVelocity[2]);

	GlobalCamera[1]->CameraDirection[0] = GlobalCamera[0]->CameraDirection[0];
	GlobalCamera[1]->CameraDirection[1] = GlobalCamera[0]->CameraDirection[1];
	GlobalCamera[1]->CameraDirection[2] = GlobalCamera[0]->CameraDirection[2];

	GlobalCamera[1]->CameraVector[0] = GlobalCamera[0]->CameraVector[0];
	GlobalCamera[1]->CameraVector[1] = GlobalCamera[0]->CameraVector[1];
	GlobalCamera[1]->CameraVector[2] = GlobalCamera[0]->CameraVector[2];

	ObjectVelocity[0] = (float)(FocusValue * 0.1);
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 50;
	MakeAlignVector(ObjectVelocity,GlobalPlayer[0].Direction[1]);

	GlobalCamera[1]->LookatPos[0] = GlobalPlayer[0].Position[0] + ObjectVelocity[0];
	GlobalCamera[1]->LookatPos[1] = GlobalPlayer[0].Position[1] + ObjectVelocity[1];
	GlobalCamera[1]->LookatPos[2] = GlobalPlayer[0].Position[2] + ObjectVelocity[2];

	ObjectVelocity[0] = (float)(FocusValue * -0.1);
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 50;
	MakeAlignVector(ObjectVelocity,GlobalPlayer[0].Direction[1]);

	GlobalCamera[0]->LookatPos[0] = GlobalPlayer[0].Position[0] + ObjectVelocity[0];
	GlobalCamera[0]->LookatPos[1] = GlobalPlayer[0].Position[1] + ObjectVelocity[1];
	GlobalCamera[0]->LookatPos[2] = GlobalPlayer[0].Position[2] + ObjectVelocity[2];

	GlobalCamera[1]->LookatVector[0] = GlobalCamera[0]->LookatVector[0];
	GlobalCamera[1]->LookatVector[1] = GlobalCamera[0]->LookatVector[1];
	GlobalCamera[1]->LookatVector[2] = GlobalCamera[0]->LookatVector[2];

	GlobalCamera[1]->ChaseDirection = GlobalCamera[0]->ChaseDirection;
	GlobalCamera[1]->OldChaseDirection = GlobalCamera[0]->OldChaseDirection;
	GlobalCamera[1]->ScreenViewAngle = GlobalCamera[0]->ScreenViewAngle;
	GlobalCamera[1]->Flag = GlobalCamera[0]->Flag;

	GlobalCamera[1]->Watch = GlobalCamera[0]->Watch;
	
	
	//DynamicObjects[ItemBoxAllocPtr[1]].Texaddr = DynamicObjects[ItemBoxAllocPtr[0]].Texaddr;
	//DynamicObjects[ItemBoxAllocPtr[1]].Indexaddr = DynamicObjects[ItemBoxAllocPtr[0]].Indexaddr;
	ItemBoxAllocPtr[1] = ItemBoxAllocPtr[0];
	
	KWDisplay2D(1);
	KWDisplay2DAfter(1);

	KWDisplay2D(2);
	KWDisplay2DAfter(2);

	KWDisplayRank(0);
	KWDisplayRank(1);
	
	TwoDFlag = 0;
}

void DrawGeometryScale(float localPosition[], short localAngle[], int localAddress, float localScale)
{
	CreateModelingMatrix(AffineMatrix,localPosition,localAngle);
	ScalingMatrix(AffineMatrix,localScale);	
	if(SetMatrix(AffineMatrix,0) == 0)
	{
		return;
	}
	gSPDisplayList(GraphPtrOffset++,localAddress);		
	
}


Vtx CoinBounds[] ={
    {   {  { 40,  40,  50}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 40, -40,  50}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40, -40,  50}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40,  40,  50}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },

    {   {  { 40,  40, 160}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 40, -40, 160}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40, -40, 160}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40,  40, 160}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 40,  40,  70}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 40, -40,  70}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40, -40,  70}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40,  40,  70}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },

    {   {  { 40,  40, 150}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  { 40, -40, 150}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40, -40, 150}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } },
    {   {  {-40,  40, 150}, 0, {0,0}, {0xff, 0xff, 0xff, 0xff} } }
};

void DrawRedCoin(float localPosition[], short localAngle[], int localAddress, float localScale)
{
	CreateModelingMatrix(AffineMatrix,localPosition,localAngle);
	ScalingMatrix(AffineMatrix,localScale);	
	if(SetMatrix(AffineMatrix,0) == 0)
	{
		return;
	}
    gSPClearGeometryMode(GraphPtrOffset++,G_LIGHTING);
    gSPVertex(GraphPtrOffset++, &CoinBounds[0], 8, 0);
    gSPCullDisplayList(GraphPtrOffset++, 0, 7);
	gSPDisplayList(GraphPtrOffset++,localAddress);		
	
}


void DrawGeometrySVectorScale(SVector localPosition, short localAngle[], int localAddress, float localScale)
{
	ObjectPosition[0] = localPosition[0];
	ObjectPosition[1] = localPosition[1];
	ObjectPosition[2] = localPosition[2];
	CreateModelingMatrix(AffineMatrix,ObjectPosition,localAngle);
	ScalingMatrix(AffineMatrix,localScale);	
	if(SetMatrix(AffineMatrix,0) == 0)
	{
		return;
	}
	
	gSPDisplayList(GraphPtrOffset++,localAddress);		
	
}

void DrawGeometry(float ObjectPosition[], short ObjectAngle[], int F3DEXAddress)
{
	DrawGeometryScale(ObjectPosition,ObjectAngle,F3DEXAddress, 1.0);
}

void DrawPaths(long RSPInput)
{
	GlobalAddressA = GetRealAddress(RSPInput);
	GlobalAddressB = 0x0D001780;

	do
	{
		ObjectAngle[0] = *(short*)(GlobalAddressA);
		ObjectAngle[1] = *(short*)(GlobalAddressA + 2);
		ObjectAngle[2] = *(short*)(GlobalAddressA + 4);

		if (ObjectPosition[0] == 0xFFFF8000)
		{
			break;
		}
		else
		{
			ObjectPosition[0] = (float)ObjectAngle[0];
			ObjectPosition[1] = (float)ObjectAngle[1];
			ObjectPosition[2] = (float)ObjectAngle[2];
			DrawGeometry(ObjectPosition,ObjectAngle,GlobalAddressB);
		}
		GlobalAddressA = GlobalAddressA + 8;
	} while (true);

}


void GFXPiranha(int PakkunAddress, float Distance)
{
	//Load Texture Block
	//
	gDPLoadTextureBlock
	(
		GraphPtrOffset++, PakkunAddress, G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
		G_TX_MIRROR,   G_TX_CLAMP ,
		G_TX_NOMASK, G_TX_NOMASK,     G_TX_NOLOD, G_TX_NOLOD
	);
	//
	//


	gSPTexture(GraphPtrOffset++, 65535, 65535,0, 0, 1);
	gDPPipeSync(GraphPtrOffset++);
	
	
	if ((HotSwapID > 0) && (OverKartHeader.FogStart > 0))
	{
		gDPSetRenderMode(GraphPtrOffset++, G_RM_FOG_SHADE_A,G_RM_AA_ZB_TEX_EDGE2);
     	gDPSetCombineMode(GraphPtrOffset++, G_CC_MODULATERGBDECALA,G_CC_PASS2);
	}
	else
	{
		gDPSetRenderMode(GraphPtrOffset++, G_RM_AA_ZB_TEX_EDGE,G_RM_AA_ZB_TEX_EDGE2);
		gDPSetCombineMode(GraphPtrOffset++, G_CC_MODULATERGBDECALA,G_CC_MODULATERGBDECALA);
	}
	
	
	

	gDPTileSync(GraphPtrOffset++);
	gDPSetTile(GraphPtrOffset++, G_IM_FMT_CI,G_IM_SIZ_8b,((32 + 7)>>3),0,0,0,G_TX_CLAMP,6,0,G_TX_MIRROR,5,0);	
	gDPSetTileSize(GraphPtrOffset++, 0,0,0,(32-1)<<2,(64-1)<<2);

	gDPSetTextureLUT(GraphPtrOffset++, G_TT_RGBA16);
	
	
	gDPSetTextureImage(GraphPtrOffset++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, &PakkunColor);
	gDPTileSync(GraphPtrOffset++);
	gDPSetTile(GraphPtrOffset++, 0, 0, 0, 256, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0);
	gDPLoadSync(GraphPtrOffset++);
	gDPLoadTLUTCmd(GraphPtrOffset++, G_TX_LOADTILE, 255);
	gDPPipeSync(GraphPtrOffset++);



	gSPVertex(GraphPtrOffset++, &PakkunV[0], 4, 0);
	gSP2Triangles(GraphPtrOffset++, 0, 1, 2, 0 ,0, 2, 3, 0);
	gDPSetTextureLUT(GraphPtrOffset++, G_TT_NONE);

	
}

void GFXTree1(float Distance)
{
	gDPSetTextureImage(GraphPtrOffset++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, &TreePalette);
	gDPTileSync(GraphPtrOffset++);
	gDPSetTile(GraphPtrOffset++, 0, 0, 0, 256, G_TX_LOADTILE, 0 , 0, 0, 0, 0, 0, 0);
	gDPLoadSync(GraphPtrOffset++);
	gDPLoadTLUTCmd(GraphPtrOffset++, G_TX_LOADTILE, 255);
	gDPPipeSync(GraphPtrOffset++);

	
	
	//fogtest
	
	if ((HotSwapID > 0) && (OverKartHeader.FogStart > 0))
	{
		gDPSetRenderMode(GraphPtrOffset++, G_RM_FOG_SHADE_A,G_RM_AA_ZB_TEX_EDGE2);
     	gDPSetCombineMode(GraphPtrOffset++, G_CC_DECALRGBA, G_CC_PASS2);
	}
	else
	{
		gDPSetRenderMode(GraphPtrOffset++, G_RM_AA_ZB_TEX_EDGE,G_RM_AA_ZB_TEX_EDGE2);
		gDPSetCombineMode(GraphPtrOffset++, G_CC_MODULATERGBA,G_CC_MODULATERGBA);
	}
	
	gSPTexture(GraphPtrOffset++, 0xFFFF,0xFFFF, 0, G_TX_RENDERTILE, G_ON);
	gDPSetTextureLUT(GraphPtrOffset++, G_TT_RGBA16);
	
	gDPTileSync(GraphPtrOffset++);
	gDPSetTile(GraphPtrOffset++, G_IM_FMT_CI,G_IM_SIZ_8b,((32 + 7)>>3),0,0,0,G_TX_CLAMP,6,0,G_TX_CLAMP,5,0);
	
	gDPSetTileSize(GraphPtrOffset++, 0,0,0,(32-1)<<2,(64-1)<<2);
	
	//Load Texture Block
	//
	gDPLoadTextureBlock
	(
		GraphPtrOffset++, 0x03009000, G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
		G_TX_CLAMP | G_TX_NOMIRROR,   G_TX_CLAMP | G_TX_NOMIRROR,
		G_TX_NOMASK, G_TX_NOMASK,     G_TX_NOLOD, G_TX_NOLOD
	);
	//
	//
	
	gSPVertex(GraphPtrOffset++, &Tree1V[0], 4, 0);
	
	gSP2Triangles(GraphPtrOffset++, 0, 1, 2, 0,		0, 2, 3, 0);
	
	gDPSetTextureLUT(GraphPtrOffset++, G_TT_NONE);
}


void DisplayPiranhaBypass(Camera* PlayerCamera, AffineMtx Affine, Object* PiranhaObject)
{
	
	int LocalPlayer = (int)( ( (Uint)(PlayerCamera) - (Uint)(&g_CameraTable) ) / (sizeof(Camera) ) );

	// Calc Distance
	float LocalDistance = 
	CheckDisplayRange(
		PlayerCamera->CameraPos,
		PiranhaObject->Position,
		PlayerCamera->CameraDirection[1],
		0,
		CameraZoom[LocalPlayer],
		16000000
	);
	//

	if 	(LocalDistance < 0)
	{
		return;
	}
	

	Affine[3][0] =  PiranhaObject->Position[0];
	Affine[3][1] =  PiranhaObject->Position[1];
	Affine[3][2] =  PiranhaObject->Position[2];
	
	if(SetMatrix(Affine,0))  
	{
		
		PakkunObject *Pakkun = (PakkunObject*)(PiranhaObject);
		int LocalCounter = (Pakkun->Timers[0] / 6);
		int PakkunAddress;
		if (LocalCounter > 8)
		{
			LocalCounter = 8;
		}
		if ((HotSwapID > 0) || (CourseID == 0))
		{
			//Custom Levels, Mario Raceway
			PakkunAddress = (LocalCounter * 2048) + GetRealAddress(0x03009800);
		}
		else
		{
			//Royal Raceway
			PakkunAddress = (LocalCounter * 2048) + GetRealAddress(0x0300A000);
		}

		GFXPiranha(PakkunAddress, LocalDistance);
	}

}
void DisplayTree1Bypass(Camera* PlayerCamera, AffineMtx Affine, Object* TreeObj)
{
	int LocalPlayer = (int)( ( (Uint)(PlayerCamera) - (Uint)(&g_CameraTable) ) / (sizeof(Camera) ) );

	// Calc Distance
	//
	float LocalDistance = 
	CheckDisplayRange(
		PlayerCamera->CameraPos,
		TreeObj->Position,
		PlayerCamera->CameraDirection[1],
		0,
		CameraZoom[LocalPlayer],
		16000000
	);
	//
	//

	if 	(LocalDistance < 0)
	{
		return;
	}
	
	if( !(TreeObj->Flag & FLYINGOBJ) && (LocalDistance < 250000) )
	{
		DisplayShadow(TreeObj->Position,TreeObj->Angle,3.0f);	
	}

	Affine[3][0] =  TreeObj->Position[0];
	Affine[3][1] =  TreeObj->Position[1];
	Affine[3][2] =  TreeObj->Position[2];
	
	if(SetMatrix(Affine,0))  
	{
		GFXTree1(LocalDistance);
	}
}
/*
void DisplayTree1Loop(Screen *PlayerScreen, AffineMtx Affine)
{
	
	bool TreeLoaded = false;
	{
	    int ThisObject;
	for (ThisObject = 0; ThisObject < MAX_OBJECT; ThisObject++)
	{
		if (SimpleObjectArray[ThisObject].Category == TREE1)
		{
			if (!TreeLoaded)
			{
				gDPLoadTLUT_pal256(GraphPtrOffset++, &TreePalette[0]);
				gDPSetCombineMode(GraphPtrOffset++, G_CC_MODULATERGBA,G_CC_MODULATERGBA);
				gDPSetRenderMode(GraphPtrOffset++, G_RM_AA_ZB_TEX_EDGE,G_RM_AA_ZB_TEX_EDGE2);
				gSPTexture(GraphPtrOffset++, 0xFFFF,0xFFFF, 0, G_TX_RENDERTILE, G_ON);
				gDPSetTextureLUT(GraphPtrOffset++, G_TT_RGBA16);
				g_Nin_SetUpTileDescrip(G_IM_FMT_CI,G_IM_SIZ_8b,32,64,0,0,G_TX_CLAMP,5,0,G_TX_CLAMP,6,0);
    			gDPLoadTextureBlock(GraphPtrOffset++, 0x06000320, G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
                             G_TX_CLAMP | G_TX_NOMIRROR,   G_TX_CLAMP | G_TX_NOMIRROR,
    	                     G_TX_NOMASK, G_TX_NOMASK,     G_TX_NOLOD, G_TX_NOLOD);
			}
		}
	}
	}
}

void DisplayObjectLoop(Screen *PlayerScreen)
{
	Camera *PlayerCamera = PlayerScreen->Camera;

	float sinB = SinT(PlayerCamera->CameraDirection[1]-(DEG1 * 180));
  	float cosB = CosT(PlayerCamera->CameraDirection[1]-(DEG1 * 180));

	AffineMatrix[0][0] =  cosB;
	AffineMatrix[1][0] =  0.0f;
	AffineMatrix[2][0] =  sinB;
	AffineMatrix[0][1] =  0.0f;
	AffineMatrix[1][1] =  1.0f;
	AffineMatrix[2][1] =  0.0f;
	AffineMatrix[0][2] = -sinB;
	AffineMatrix[1][2] =  0.0f;
	AffineMatrix[2][2] =  cosB;
	AffineMatrix[0][3] =  0.0f;
	AffineMatrix[1][3] =  0.0f;
	AffineMatrix[2][3] =  0.0f;
	AffineMatrix[3][3] =  1.0f;

	if (GameMode != GAMEMODE_BATTLE)
	{
		DisplayFlagGate(PlayerCamera);
	}

	
}*/
