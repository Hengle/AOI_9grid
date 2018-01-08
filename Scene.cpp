/*
author: atao 1628025718@qq.com
*/
#include "Scene.h"
using namespace TT;

//��ʼ������9��������
void Scene::InitGrids()
{
	int width = width_;
	int height = height_;
	//�������е�ÿ�����궼ת��Ϊ9��������
	for (int w = 0; w <= width; ++w)
	{
		for (int h = 0; h <= height; ++h)
		{
			int gid = GetGid(w, h);
			auto it = grids_.find(gid);
			//û�г�ʼ�����ͳ�ʼ��һ��
			if (it == grids_.end()) 
			{
				//std::vector<uint64_t> grid_entities = {};
				grids_.insert({ gid, {} });
				//printf("insert %d\n", gid);
			}
			
		}
	}
}

//debug
void Scene::Stat() const {
	std::cout << grids_.size() << std::endl;
}

//���볡��
void Scene::Enter(const Entity& entity)
{
	//ʵ����볡�������Լ��������Ϣ�㲥����Ӧ������
	//ͬʱ����Ӧ����������ʵ���״̬ͬ����ʵ��
	int gid = GetGid(entity.x(), entity.y());
	Enter(entity.id(), gid);
	EnterBroadcast(entity, entity.x(), entity.y());
}

void Scene::Enter(const uint64_t id, const int gid)
{
	auto it = grids_.find(gid);
	if (it != grids_.end())
	{
		auto find_it = it->second.find(id);
		if (find_it == it->second.end())
		{
			it->second.insert(id);
		}
	}
}
// �뿪����
void Scene::Leave(const Entity& entity)
{
	//ʵ���뿪����������Ұ
	//�����뿪֪ͨ����Ұ�ڵ�������Ұ�����¸������ʵ�弯������
	int gid = GetGid(entity.x(), entity.y());
	Leave(entity.id(), gid);
	LeaveBroadcast(entity, entity.x(), entity.y());
}

void Scene::Leave(const uint64_t id, const int gid)
{
	auto it = grids_.find(gid);
	if (it != grids_.end())
	{
		auto find_it = it->second.find(id);
		if (find_it != it->second.end())
		{
			it->second.erase(id);
		}
	}
}

//��������£�ʵ���λ����Ϣ�Լ����º�ά�����������������ʵ�������
void Scene::Move(const Entity& entity, const int x, const int y)
{
	//���������������ƶ���Ϣ
	MoveBroadcast(entity, x, y);
	//�ƶ���ʱ��Ҫ�����Ƿ����ӣ����û�п���ӣ�ֻ��Ҫ����Ұ����㲥�ƶ����Ϳ��ԡ�
	//�������ӵĻ�����Ҫ���뿪������㲥�뿪��Ϣ��ͬʱ����������㲥��������ͬʱ�����������򶼹㲥�ƶ���Ϣ��
	//ͨ��ʵ�ֵ�ʱ���뿪��Ϣ����ʡ�Ե�����Ϊʵ���ƶ��߳�����Ұ�Ļ����ͻ����Լ��������أ����٣�ʵ�����
	int old_gid = GetGid(entity.x(), entity.y());
	int new_gid = GetGid(x, y);
	if (old_gid == new_gid)
	{
		//printf("û�п����\n");
		return;
	}
	else
	{
		//�ƶ����뿪��ԭ���Ĵ���ӣ���Ұ�仯
		int gx1 = entity.x() / GX;
		int gy1 = entity.y() / GY;
		int gx2 = x / GX;
		int gy2 = y / GY;
		
		Scene::DIR dir = GetDir(entity.x(), entity.y(), x, y);
		//���ݲ�ͬ�ķ���ֱ�����ƶ�
		switch (dir)
		{
		case TT::Scene::DIR_UP:
			move_up(entity, x, y);
			break;
		case TT::Scene::DIR_DOWN:
			move_down(entity, x, y);
			break;
		case TT::Scene::DIR_LEFT:
			move_left(entity, x, y);
			break;
		case TT::Scene::DIR_RIGHT:
			move_right(entity, x, y);
			break;
		case TT::Scene::DIR_LEFT_UP:
			move_left_up(entity, x, y);
			break;
		case TT::Scene::DIR_RIGHT_UP:
			move_right_up(entity, x, y);
			break;
		case TT::Scene::DIR_LEFT_DOWN:
			move_left_down(entity, x, y);
			break;
		case TT::Scene::DIR_RIGHT_DOWN:
			move_right_down(entity, x, y);
			break;
		default:
			std::cerr << "������ƶ�Ŀ�ĵ�" << std::endl;
			break;
		}
	}
}

//֪ͨ�뿪
void SendLeave(const Entity& entity, const std::unordered_set<uint64_t>& ids)
{
	for (auto id : ids)
	{
		//TODO ����������Ƿ�һ���뿪�������͸���Ӧid��ʵ�壬����ֱ�ӷ���id��Ӧ����������
		std::cout << "notity: " << id << " " << entity.id() << " leave." << std::endl;
	}
}
//֪ͨ����
void SendEnter(const Entity& entity, const std::unordered_set<uint64_t>& ids)
{
	for (auto id : ids)
	{
		//TODO ����������ǽ�entity���Ȼ���͸���Ӧid��ʵ�壬����ֱ�ӷ���id��Ӧ����������
		//std::cout << "notity: " << id << " " << entity.id() << " enter." << std::endl;
	}
}
void Scene::EnterBroadcast(const Entity& entity, const int x, const int y)
{
	std::unordered_set<uint64_t> entities;
	ViewEntities(entities, x, y);
	//����Ұ�ڵ�ʵ��ͨ��entity����
	SendEnter(entity, entities);
}

void Scene::LeaveBroadcast(const Entity& entity, const int x, const int y)
{
	std::unordered_set<uint64_t> entities;
	ViewEntities(entities, entity.x(), entity.y());
	//����Ұ�ڵ�ʵ��ͨ��entity�뿪
	SendLeave(entity, entities);
}

void Scene::MoveBroadcast(const Entity& entity, const int x, const int y)
{
	std::unordered_set<uint64_t> entities;
	ViewEntities(entities, x, y);
	//printf("֪ͨ�ƶ���Ϣ�Ķ�������:%d\n", entities.size());
	//����Ұ�ڵ�ʵ��ͨ��entity����
	for (auto id : entities)
	{
		//TODO �����ƶ���Ϣ
		std::cout << "notity: " << id 
			<< " " << entity.id() 
			<< " move to " << x << " " << y 
			<< std::endl;
	}
}

int Scene::GetGid(const int x, const int y) const
{
	
	int gx = x / GX;
	int gy = y / GY;
	return GxGy2Gid(gx, gy);
}

int Scene::GxGy2Gid(const int gx, const int gy) const
{
	int gx1 = gx < 0 ? 0 : gx;
	int gy1 = gy < 0 ? 0 : gy;
	return gx1 * FACTOR + gy1;
}

//��Ұ�ڵ�ʵ��
void Scene::ViewEntities(std::unordered_set<uint64_t>& entities, const int x, const int y)
{
	std::unordered_set<int> view_grids;
	ViewGrids(view_grids, x, y);
	for (auto gid : view_grids)
	{
		auto grid_it = grids_.find(gid);
		if (grid_it != grids_.end())
		{
			for (auto grid_entity_id : grid_it->second)
			{
				entities.insert(grid_entity_id);
			}
		}
	}
}

//��Ұ�ڵ�9����
//��Ҫע����ǣ����и�����Ϊ��1֮������Ѿ��Ƿ���λ���ˣ�������ʹ�ý����ʱ����Ҫ���л������ж�
void Scene::ViewGrids(std::unordered_set<int>& view_grids, const int x, const int y)
{
	int gx = x / GX;
	int gy = y / GY;

	//��9�����Ӿ�����Ұ
	view_grids.insert({ 
		GxGy2Gid(gx, gy + 1)      //���Ϸ�
		,GxGy2Gid(gx - 1, gy + 1) //���Ϸ�
		,GxGy2Gid(gx + 1, gy + 1) //���Ϸ�
		,GxGy2Gid(gx, gy)         //�м�
		,GxGy2Gid(gx - 1, gy)     //���
		,GxGy2Gid(gx + 1, gy)     //�ұ�
		,GxGy2Gid(gx, gy - 1)     //���·�
		,GxGy2Gid(gx - 1, gy - 1) //���·�
		,GxGy2Gid(gx + 1, gy - 1) //���·�
	});
}

void Scene::GridEntities(std::unordered_set<uint64_t>& entities, int gid)
{
	auto grid_it = grids_.find(gid);
	if (grid_it != grids_.end())
	{
		for (auto grid_entity_id : grid_it->second)
		{
			entities.insert(grid_entity_id);
		}
	}
}

//��{x1,y1}��{x2,y2}�ƶ�
Scene::DIR Scene::GetDir(const int x1, const int y1, const int x2, const int y2)
{
	//��ֱ�����ƶ�
	if (x1 == x2)
	{
		if (y2 > y1)
			return Scene::DIR::DIR_UP;
		else
			return Scene::DIR::DIR_DOWN;
	}

	//ˮƽ�ƶ�
	if (y1 == y2)
	{
		if (x2 > x1)
			return Scene::DIR::DIR_RIGHT;
		else
			return Scene::DIR::DIR_LEFT;
	}

	//����4������
	if (x2 > x1 && y2 > y1)
		return DIR_RIGHT_UP;
	if (x2 > x1 && y2 < y1)
		return Scene::DIR::DIR_RIGHT_DOWN;
	if (x1 > x2 && y2 > y1)
		return Scene::DIR::DIR_LEFT_UP;
	if (x1 > x2 && y2 < y1)
		return Scene::DIR::DIR_LEFT_DOWN;

	return Scene::DIR::DIR_NULL;
}

//������Ӧ����Ϣ֪ͨ
void Scene::Notity(const Entity& entity,
	const std::unordered_set<int>& leave_grids, 
	const std::unordered_set<int>& enter_grids)
{
	std::unordered_set<uint64_t> leave_entities, enter_entities;
	for (auto it : leave_grids)
	{
		//Leave(entity.id(), it);
		GridEntities(leave_entities, it); //��ȡ��Ҫ֪ͨʵ���뿪��Ϣ������ʵ��
	}
	for (auto it : enter_grids)
	{
		//Enter(entity.id(), it);
		GridEntities(enter_entities, it); //��ȡ��Ҫ֪ͨʵ�������Ϣ������ʵ��
	}
	SendLeave(entity, leave_entities);
	SendEnter(entity, enter_entities);
}

//�����ƶ���������3�����ӻ��뿪��Ұ���������3���µĸ��ӽ�����Ұ
//���漸�����������ƣ���Ҫע��ĵ��ViewGrids����һ������ʹ�ý��ǰ���л����ĺϷ����ж�
void Scene::move_up(const Entity& entity, int x, int y)
{
	//������뿪�Ĵ����
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1, gy1-1),    //�·�
		GxGy2Gid(gx1-1,gy1-1), //���·�
		GxGy2Gid(gx1+1,gy1-1)  //���·�
	});
	enter_grids.insert({
		GxGy2Gid(gx2, gy2+1),    //�Ϸ�
		GxGy2Gid(gx2 - 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 + 1,gy2 + 1)  //���Ϸ�
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_down(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1, gy1 + 1),    //�Ϸ�
		GxGy2Gid(gx1 - 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 + 1,gy1 + 1)  //���Ϸ�
	});
	enter_grids.insert({
		GxGy2Gid(gx2, gy2 - 1),    //�·�
		GxGy2Gid(gx2 - 1,gy2 - 1), //���·�
		GxGy2Gid(gx2 + 1,gy2 - 1)  //���·�
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_left(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1, gy1 + 1),    //�ҷ�
		GxGy2Gid(gx1 + 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 + 1,gy1 - 1)  //���·�
		
	});
	enter_grids.insert({
		GxGy2Gid(gx2 - 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 - 1,gy2 - 1), //���·�
		GxGy2Gid(gx2 - 1,gy1), //��
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_right(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1 - 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 - 1,gy1 - 1), //���·�
		GxGy2Gid(gx1 - 1,gy1), //��
	});
	enter_grids.insert({
		GxGy2Gid(gx2, gy2 + 1),    //�ҷ�
		GxGy2Gid(gx2 + 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 + 1,gy2 - 1)  //���·�
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_left_up(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1,gy1 - 1),    //�·�
		GxGy2Gid(gx1 - 1,gy1 - 1),    //���·�
		GxGy2Gid(gx1 + 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 + 1,gy1 - 1),  //���·�
		GxGy2Gid(gx1 + 1,gy1) //�ұ�
	});
	enter_grids.insert({
		GxGy2Gid(gx2 - 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 - 1,gy2 - 1), //���·�
		GxGy2Gid(gx2 - 1,gy2), //��
		GxGy2Gid(gx2 + 1,gy2 + 1), //����
		GxGy2Gid(gx2,gy2 + 1) //��
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_right_up(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1,gy1 - 1),    //�·�
		GxGy2Gid(gx1 + 1,gy1 - 1),  //���·�
		GxGy2Gid(gx1 - 1,gy1 - 1),    //���·�
		GxGy2Gid(gx1 - 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 - 1,gy1) //���
	});
	enter_grids.insert({
		GxGy2Gid(gx2 - 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2,gy2 + 1), //��
		GxGy2Gid(gx2 + 1,gy2 + 1), //����
		GxGy2Gid(gx2 + 1,gy2), //�ҷ�
		GxGy2Gid(gx2 + 1,gy2 - 1) //���·�
	});
	//printf("gx1 %d gy1 %d gx2 %d gy2 %d\n", gx1, gy1, gx2, gy2);
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_left_down(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1 - 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1,gy1 + 1), //��
		GxGy2Gid(gx1 + 1,gy1 + 1), //����
		GxGy2Gid(gx1 + 1,gy1), //�ҷ�
		GxGy2Gid(gx1 + 1,gy1 - 1) //���·�
	});
	enter_grids.insert({
		GxGy2Gid(gx2,gy2 - 1),    //�·�
		GxGy2Gid(gx2 + 1,gy2 - 1),  //���·�
		GxGy2Gid(gx2 - 1,gy2 - 1),    //���·�
		GxGy2Gid(gx2 - 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 - 1,gy2) //���
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}
void Scene::move_right_down(const Entity& entity, int x, int y)
{
	std::unordered_set<int> leave_grids, enter_grids;
	int gx1 = entity.x() / GX;
	int gy1 = entity.y() / GY;
	int gx2 = x / GX;
	int gy2 = y / GY;
	leave_grids.insert({
		GxGy2Gid(gx1 - 1,gy1 + 1), //���Ϸ�
		GxGy2Gid(gx1 - 1,gy1 - 1), //���·�
		GxGy2Gid(gx1 - 1,gy1), //��
		GxGy2Gid(gx1 + 1,gy1 + 1), //����
		GxGy2Gid(gx1,gy1 + 1) //��
	});
	enter_grids.insert({
		GxGy2Gid(gx2,gy2 - 1),    //�·�
		GxGy2Gid(gx2 - 1,gy2 - 1),    //���·�
		GxGy2Gid(gx2 + 1,gy2 + 1), //���Ϸ�
		GxGy2Gid(gx2 + 1,gy2 - 1),  //���·�
		GxGy2Gid(gx2 + 1,gy2) //�ұ�
	});
	Leave(entity.id(), GxGy2Gid(gx1, gy1));
	Enter(entity.id(), GxGy2Gid(gx2, gy2));
	Notity(entity, leave_grids, enter_grids);
}