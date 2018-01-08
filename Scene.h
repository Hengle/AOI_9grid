/*
author: atao 1628025718@qq.com
*/
#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "Entity.h"


namespace TT
{
//����ͨ����ת��Ϊ9���������ת������
//���ֵĻ���ԭ�򣺽�һ����Ұ�������Դ��ڣ��ڿ�͸���9�ȷ֣��Ϳ��Եõ�GX,GY��ʵ�ʴ�С��
#define GX 8
#define GY 6
//�������100����ȫ���Խ�һ��100*100��2D����ת����9�����ͼ��ÿ������Ψһ��ʾ��
//�����и����ɣ�gy����Ǽ�λ��������100�����y���꣬GY=6��ʱ����ôgy������16����2λ���� gx�ͳ���10^2�ټ���gy��ֵ�Ϳ��Թ���Ψһgid�ˡ�
#define FACTOR 100

	// ����
	class Scene
	{
	public:
		Scene(int width=100, int height=100) :
			width_(width), height_(height)
		{
			InitGrids();
		}
		Scene() = delete;
		~Scene() {};
	public:
		//aoi api
		//��������AOI��صĽӿڣ�����ʵ����롢�뿪���ڳ������ƶ�
		void Enter(const Entity& entity);
		void Enter(const uint64_t id, const int gid);
		void Leave(const Entity& entity);
		void Leave(const uint64_t id, const int gid);
		void Move(const Entity& entity, const int x, const int y);
		// debug api
		int width() const { return width_; }
		int height() const { return height_; }
		void Stat() const;
	
	private:
		int width_;  //������
		int height_; //������
		//9����
		//����Ӳ���{GX,GY}��ʾ��Ϊ�˴洢����ʹ��,��GX��GYת��Ϊһ��Gid��������ʾʹ�ã�GX*100+GY = Gid��������Ը��ݳ���ʵ�ʴ�С
		//������Ӧ�Ĺ�ʽ����ȻҲ����ֱ�Ӵ洢һ��tuple��
		//ÿ��gid��Ӧһ����Ұ�е�һ��������Ϊ�漰��Ƶ���Ĳ��Һ�ɾ��������set�Ǻ��ʺϵ����ݽṹ
		typedef int Gid;
		typedef std::unordered_map<Gid, std::unordered_set<uint64_t>> Grids;
		Grids grids_;
		//8������
		enum DIR {
			DIR_NULL = 0, //��������Ϸ����
			DIR_UP = 1,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_LEFT_UP,
			DIR_RIGHT_UP,
			DIR_LEFT_DOWN,
			DIR_RIGHT_DOWN = 8
		};

	private:
		//��ʼ��9��������
		void InitGrids();
		//��Ӧ��AOI�㲥
		void EnterBroadcast(const Entity& entity, const int x, const int y);
		void LeaveBroadcast(const Entity& entity, const int x, const int y);
		void MoveBroadcast(const Entity& entity, const int x, const int y);
		//����ʵ���Ӧ��Gid
		int GetGid(const int x, const int y) const;
		//�Ź������굽�Ź���id��ת��
		int GxGy2Gid(const int gx, const int gy) const;
		//��Ұ�ڵ�ʵ��
		void ViewEntities(std::unordered_set<uint64_t>& entities, const int x, const int y);
		//��Ұ�ڵĸ��ӣ�����ӣ�
		void ViewGrids(std::unordered_set<int>& view_grids, const int x, const int y);
		//�����ڵ�ʵ��
		void GridEntities(std::unordered_set<uint64_t>& entities, int gid);
		void Notity(const Entity& entity, const std::unordered_set<int>& leave_grids, const std::unordered_set<int>& enter_grids);
		//�����ƶ��ķ���
		DIR GetDir(const int x1, const int y1, const int x2, const int y2);
		//8�������ϵ��ƶ�
		void move_up(const Entity& entity, int x, int y);
		void move_down(const Entity& entity, int x, int y);
		void move_left(const Entity& entity, int x, int y);
		void move_right(const Entity& entity, int x, int y);
		void move_left_up(const Entity& entity, int x, int y);
		void move_right_up(const Entity& entity, int x, int y);
		void move_left_down(const Entity& entity, int x, int y);
		void move_right_down(const Entity& entity, int x, int y);

	};
}//namespace TT

