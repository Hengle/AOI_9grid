/*
author: atao 1628025718@qq.com
*/
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Scene.h"
#include "Entity.h"
using namespace TT;
int main()
{
	//100x100�ĳ�����Ҳ����10000��С����
	//ת����9����󣬱��200��������
	TT::Scene scene(100, 100);

	//׼��10000��ʵ��,����ֱ���(0,0),(0,1),...(99,99)
	std::vector<Entity> e_vec;
	e_vec.reserve(10000);
	for (int x = 0; x < 100; ++x)
	{
		for (int y = 0; y < 100; ++y)
		{
			Entity e(x * 100 + y, x, y);
			e_vec.push_back(e);
		}
	}
	//ȫ�����볡��
	for (auto it : e_vec)
	{
		scene.Enter(it);
	}
	scene.Stat();
	//printf("66th id %I64u x %d y %d\n", e_vec[66].id(), e_vec[66].x(), e_vec[66].y());
	getchar();
	//scene.Leave(e2);
	//�����ƶ�
	for (int i = 1; i < 1000; ++i)
	{
		scene.Move(e_vec[0], i, i);
		getchar();
		e_vec[0].set_x(i);
		e_vec[0].set_y(i);
	}
	getchar();
}