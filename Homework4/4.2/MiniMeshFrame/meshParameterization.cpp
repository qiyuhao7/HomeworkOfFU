#include "meshParameterization.h"
#define PI 3.1415926
MeshParameterization::MeshParameterization(MyMesh meshIn)
{
	mesh = meshIn;
	meshResult = mesh;
	N = mesh.n_vertices();
	n = N;
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
	{
		auto is_boundary = mesh.is_boundary(it);
		if (is_boundary)
		{
			n--;
		}
	}
	cout << "N is:" << N << "n is:" << n << endl;

}

MeshParameterization::~MeshParameterization()
{

}

MyMesh MeshParameterization::uniform()
{
	//ip = internal point
	//vector<MyMesh::Point>  ips;
	//vector<MyMesh::Point>::iterator ip_it;

	//MyMesh::Point               bp;
	//MyMesh::Point               bpi;
	MyMesh::VertexIter          v_it;
	MyMesh::VertexVertexIter    vv_it;

	OpenMesh::VPropHandleT<int> id;
	mesh.add_property(id);

	MatrixXd A(n, n);
	MatrixXd lamda(n, N);

	int i = 0;

	//计算id
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			mesh.property(id, *v_it) = i;

			i++;
		}
	}

	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				mesh.property(id, hv) = i;
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}

	//cout << endl;
	//for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	//{
	//	cout << mesh.property(id, *v_it) << endl;

	//}

	//cout << i << endl;

	//初始化A和lamda

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			A(i, j) = 0;
		}
	}

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			lamda(i, j) = 0;
		}
	}


	//构造A 和 lamda
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{


			//computer di
			int di = 0;
			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{
				di++;
			}

			double lamdaij = 1.0 / di;

			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{
				int test = mesh.property(id, *vv_it);
				lamda(i, mesh.property(id, *vv_it)) = lamdaij;
			}

			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{

				if (!mesh.is_boundary(*vv_it))
				{
					int j = mesh.property(id, *vv_it);
					//cout << vv_it.handle() << endl;
					A(i, j) = -lamda(i, j);
				}


			}

			A(i, i) = 1;

			i++;
		}

	}

	//计算b1,b2
	double b_num = N - n;
	double e_num = b_num / 4 + 1;
	double t = 0;
	VectorXd bu(N - n); //x
	VectorXd bv(N - n); //y
	VectorXd b1(n);
	VectorXd b2(n);

	MatrixXd lamdaN_n(n, N - n);
	for (size_t i = 0; i < e_num; i++)
	{
		bu(i) = 0;
		bv(i) = 1 - i / e_num;
	}

	for (size_t i = e_num; i < 2 * e_num; i++)
	{
		bu(i) = t / e_num;
		bv(i) = 0;
		t++;
	}
	t = 0;
	for (size_t i = 2 * e_num; i < 3 * e_num; i++)
	{
		bu(i) = 1;
		bv(i) = t / e_num;
		t++;
	}
	t = 0;
	for (size_t i = 3 * e_num; i < (N - n); i++)
	{
		bu(i) = 1 - t / e_num;
		bv(i) = 1;
		t++;
	}

	//for (size_t i = 0; i < b_num; i++)
	//{
	//	cout << bu[i] << " " << bv[i] << endl;
	//}


	//从0，n开始每行取N-n个元素，每列取n个元素

	int test = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N - n; j++)
		{
			double tmp = lamda(i, j + n);
			if (tmp > 0)
				test++;
			lamdaN_n(i, j) = lamda(i, j + n);
		}
	}



	//cout << "test" << endl;

	b1 = lamdaN_n*bu;
	b2 = lamdaN_n*bv;
	//cout << b1 << endl;
	//cout << b2 << endl;
	//最后求解

	VectorXd resultU(n);
	VectorXd resultV(n);

	resultU = A.householderQr().solve(b1);
	resultV = A.householderQr().solve(b2);

	//cout << resultU << endl;
	//cout << resultV << endl;
	MyMesh::Point               result;
	mesh.request_vertex_texcoords2D();
	MyMesh::TexCoord2D;
	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_texcoord2D(*v_it, OpenMesh::Vec2f(result[0], result[1]));
			i++;
		}
	}

	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_texcoord2D(hv, OpenMesh::Vec2f(result[0], result[1]));

				i++;
			} while (hv != v_it.handle());
			break;
		}

	}

	OpenMesh::IO::Options opt = OpenMesh::IO::Options::VertexTexCoord;
	OpenMesh::IO::write_mesh(mesh, "uniform.obj", opt);

	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_point(*v_it, result);
			i++;
		}
	}
	//边界点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_point(hv, result);
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}
	//废弃代码
	//i = 0;
	//for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	//{
	//	auto is_boundary = mesh.is_boundary(*v_it);

	//	if (is_boundary)
	//	{
	//		////pushback ip
	//		//ip = mesh.point(*v_it);
	//		//ips.push_back(ip);
	//		//
	//		//计算nid属性
	//		result[0] = bu[i];
	//		result[1] = bv[i];
	//		result[2] = 0.0;
	//		//mesh.property(id, *v_it) = i;
	//		mesh.set_point(*v_it, result);
	//		i++;
	//	}
	//}

	return  mesh;
}

MyMesh MeshParameterization::weightedLeastSquares()
{
	MyMesh::VertexIter          v_it;
	MyMesh::VertexVertexIter    vv_it;

	OpenMesh::VPropHandleT<int> id;
	mesh.add_property(id);

	MatrixXd A(n, n);
	MatrixXd lamda(n, N);
	VectorXd w(N);
	int i = 0;

	//计算id
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			mesh.property(id, *v_it) = i;

			i++;
		}
	}

	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				mesh.property(id, hv) = i;
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}

	//cout << endl;
	//for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	//{
	//	cout << mesh.property(id, *v_it) << endl;

	//}

	//cout << i << endl;

	//初始化A和lamda

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			A(i, j) = 0;
		}
	}

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			lamda(i, j) = 0;
		}
	}


	//构造A 和 lamda
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			auto point1 = mesh.point(v_it);
			//computer di
			int di = 0;
			double sum_w = 0.0;
			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{
				auto point2 = mesh.point(vv_it);
				double distance = (point1[0] - point2[0])*(point1[0] - point2[0]) + (point1[1] - point2[1])*(point1[1] - point2[1]) + (point1[2] - point2[2])*(point1[2] - point2[2]);
				distance = sqrt(distance);
				double wij = 1.0 / distance;
				w(mesh.property(id, *vv_it)) = wij;
				sum_w += wij;
			}



			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{
				int j = mesh.property(id, *vv_it);
				lamda(i, j) = w(j) / sum_w;
			}

			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{

				if (!mesh.is_boundary(*vv_it))
				{
					int j = mesh.property(id, *vv_it);
					//cout << vv_it.handle() << endl;
					A(i, j) = -lamda(i, j);
				}


			}

			A(i, i) = 1;

			i++;
		}

	}

	//计算b1,b2
	double b_num = N - n;
	double e_num = b_num / 4 + 1;
	double t = 0;
	VectorXd bu(N - n); //x
	VectorXd bv(N - n); //y
	VectorXd b1(n);
	VectorXd b2(n);

	MatrixXd lamdaN_n(n, N - n);
	for (size_t i = 0; i < e_num; i++)
	{
		bu(i) = 0;
		bv(i) = 1 - i / e_num;
	}

	for (size_t i = e_num; i < 2 * e_num; i++)
	{
		bu(i) = t / e_num;
		bv(i) = 0;
		t++;
	}
	t = 0;
	for (size_t i = 2 * e_num; i < 3 * e_num; i++)
	{
		bu(i) = 1;
		bv(i) = t / e_num;
		t++;
	}
	t = 0;
	for (size_t i = 3 * e_num; i < (N - n); i++)
	{
		bu(i) = 1 - t / e_num;
		bv(i) = 1;
		t++;
	}

	//for (size_t i = 0; i < b_num; i++)
	//{
	//	cout << bu[i] << " " << bv[i] << endl;
	//}


	//从0，n开始每行取N-n个元素，每列取n个元素

	int test = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N - n; j++)
		{
			double tmp = lamda(i, j + n);
			if (tmp > 0)
				test++;
			lamdaN_n(i, j) = lamda(i, j + n);
		}
	}



	//cout << "test" << endl;

	b1 = lamdaN_n*bu;
	b2 = lamdaN_n*bv;
	//cout << b1 << endl;
	//cout << b2 << endl;
	//最后求解

	VectorXd resultU(n);
	VectorXd resultV(n);

	resultU = A.householderQr().solve(b1);
	resultV = A.householderQr().solve(b2);

	//cout << resultU << endl;
	//cout << resultV << endl;
	MyMesh::Point               result;
	mesh.request_vertex_texcoords2D();
	MyMesh::TexCoord2D;
	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_texcoord2D(*v_it, OpenMesh::Vec2f(result[0], result[1]));
			i++;
		}
	}

	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_texcoord2D(hv, OpenMesh::Vec2f(result[0], result[1]));

				i++;
			} while (hv != v_it.handle());
			break;
		}

	}

	OpenMesh::IO::Options opt = OpenMesh::IO::Options::VertexTexCoord;
	OpenMesh::IO::write_mesh(mesh, "WLS.obj", opt);

	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_point(*v_it, result);
			i++;
		}
	}
	//边界点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_point(hv, result);
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}

	return mesh;
}

MyMesh MeshParameterization::shapePreserving()
{

	MyMesh::VertexIter          v_it;
	MyMesh::VertexVertexIter    vv_it;
	
	OpenMesh::VPropHandleT<int> id;
	mesh.add_property(id);

	MatrixXd A(n, n);

	VectorXd p(N);
	MatrixXd lamda(n, N);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			A(i, j) = 0;
		}
	}

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			lamda(i, j) = 0;
		}
	}

	int i = 0;

	//计算id
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			mesh.property(id, *v_it) = i;

			i++;
		}
	}

	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				mesh.property(id, hv) = i;
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}

	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			auto point1 = mesh.point(*v_it);
			//computer di
			int di = 1;
			vector<MyMesh::Point>  xs;
			vector<MyMesh::Point>  ps;
			vector<double> radius;
			vector<double> theta;
			theta.push_back(0.0);
			//theta.push_back(0.0);
			xs.push_back(point1);
			radius.push_back(1.0);
			double theta_sum = 0.0;
			MyMesh::Normal n1, n2;
			//建立xs theta radius theta_sum
			bool isfirst = true;
			for (auto vv_ccwit = mesh.vv_ccwiter(*v_it); vv_ccwit.is_valid(); vv_ccwit++)
			{
				auto point = mesh.point(*vv_ccwit);
				xs.push_back(point);

				double radius_tmp = (xs[di] - xs[0]) | (xs[di] - xs[0]);
				radius_tmp = sqrt(radius_tmp);
				radius.push_back(radius_tmp);


				n1 = (xs[di] - xs[0])/radius[di];
				//cout << n1.length() <<endl;
				n2 = (xs[di - 1] - xs[0]) / radius[di - 1];
				//cout << n2.length() << endl;

				double theta_tmp;
				theta_tmp = acos(n1 | n2);
				theta.push_back(theta_tmp);

				di++;
			}
			di--;
			n1 = (xs[di] - xs[0]) / radius[di];
			//cout << n1.length() << endl;
			n2 = (xs[1] - xs[0]) / radius[1];
			//cout << n2.length() << endl;
			double theta_tmp = n1 | n2;
			theta_tmp = acos(theta_tmp);
			theta[1] = theta_tmp;
			

			//建立p angle

			double angle_sum = 0.0;
			MyMesh::Point p;
			p[0] = p[1] = p[2] = 0.0;
			ps.push_back(p);
			theta_sum = 0.0;
			for (int i = 1; i <= di; i++)
			{
				theta_sum += theta[i];
			}


			//初始化第一个点
			p[0] = radius[1];
			ps.push_back(p);
			//cout << endl;

			double angle_tmp = 0.0;
			for (int i = 2; i <= di; i++)
			{
				angle_tmp = 2.0*PI* theta[i] / theta_sum;
				angle_sum += angle_tmp;
				p[0] = radius[i] * cos(angle_sum);
				p[1] = radius[i] * sin(angle_sum);
				ps.push_back(p);
			}


			//for (size_t i = 1; i <= di; i++)
			//{
			//	cout << ps[i][0] << " " << ps[i][1] << endl;
			//}

			//vector<MyMesh::Point>  ips;

			//MyMesh::Normal pt(1, -3, 2), qt(0, 2, 4);
			//cout << "dot product    : " << (pt | qt) << endl;
			//cout << "cross product  : " << (pt % qt) << endl;
			//cout << (pt % qt).length() << endl;

			MyMesh::Normal p1p, p2p,p3p,p1p2,p1p3,p2p3;
			float pp1p2, pp2p3, pp1p3,p1p2p3;
			MatrixXd miu(di+1, di+1);
			for (size_t i = 0; i <= di; i++)
			{
				for (size_t j = 0; j <= di; j++)
				{
					miu(i, j) = 0;
				}
			}
			MatrixXd A(3, 3);
			A << 0, 0, 0, 0, 0, 0, 1, 1, 1;
			Vector3d x(0,0,0);
			Vector3d b(0,0,1);
			for (size_t i = 1; i <= di; i++)
			{
				for (size_t j = 1; j <= di; j++)
				{
					int j2 = j + 1;
					if (j == di)
					{
						j2 = 1;
					}
					if ((i == j) || (i == (j + 1)%di))
					{
						continue;
					}
					else
					{
						p1p = ps[i] - ps[0];
						p2p = ps[j] - ps[0];
						p3p = ps[j2] - ps[0];
						p1p2 = ps[j] - ps[i];
						p1p3 = ps[j2] - ps[i];

						pp1p2 = (p1p%p2p).length();
						pp2p3 = (p2p%p3p).length();
						pp1p3 = (p1p%p3p).length();
						p1p2p3 = (p1p2%p1p3).length();
						// abs(p1p2p3 - (pp1p2 + pp2p3 + pp1p3))<0.0001
						if (p1p2p3 >= (pp1p2 + pp2p3 + pp1p3) || abs(p1p2p3 - (pp1p2 + pp2p3 + pp1p3))<0.000001)
						{

							A(0, 0) = ps[i][0];
							A(1, 0) = ps[i][1];
							A(0, 1) = ps[j][0];
							A(1, 1) = ps[j][1];
							A(0, 2) = ps[j2][0];
							A(1, 2) = ps[j2][1];
							//cout << A << endl;
							x = A.colPivHouseholderQr().solve(b);
							//if (x[0] < 0.0001 || x[1] < 0.0001 || x[2] < 0.0001)
							//{
							//	continue;
							//}

							miu(i, i) = x[0];
							miu(j, i) = x[1];
							miu(j2, i) = x[2];


							break;
						}
					}
				}
			}

			//cout << miu << endl;


			/*MatrixXd A(2, di);
			VectorXd b(di);
			Vector2d zero;

			for (size_t i = 0; i < di; i++)
			{
				A(0, i) = ps[i + 1][0];
				A(1, i) = ps[i + 1][1];
			}

			b = A.householderQr().solve(zero);
			double test = 0;
			for (size_t i = 0; i < di; i++)
			{
				test += b[i];
			}

			*/
			
			int t = 1;
			for (auto vv_ccwit = mesh.vv_ccwiter(*v_it); vv_ccwit.is_valid(); vv_ccwit++)
			{
				double sum = 0;
				for (size_t s = 1; s <= di; s++)
				{
					sum += miu(t,s);
				}
				lamda(i, mesh.property(id, *vv_ccwit)) = sum / di;

				t++;
			}

			i++;
		}
	}

	
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{

			auto point1 = mesh.point(v_it);
			//computer di


			for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++)
			{

				if (!mesh.is_boundary(*vv_it))
				{
					int j = mesh.property(id, *vv_it);
					//cout << vv_it.handle() << endl;
					A(i, j) = -lamda(i, j);
				}


			}

			A(i, i) = 1;

			i++;
		}

	}


	//计算b1,b2
	double b_num = N - n;
	double e_num = b_num / 4 + 1;
	double t = 0;
	VectorXd bu(N - n); //x
	VectorXd bv(N - n); //y
	VectorXd b1(n);
	VectorXd b2(n);

	MatrixXd lamdaN_n(n, N - n);
	for (size_t i = 0; i < e_num; i++)
	{
		bu(i) = 0;
		bv(i) = 1 - i / e_num;
	}

	for (size_t i = e_num; i < 2 * e_num; i++)
	{
		bu(i) = t / e_num;
		bv(i) = 0;
		t++;
	}
	t = 0;
	for (size_t i = 2 * e_num; i < 3 * e_num; i++)
	{
		bu(i) = 1;
		bv(i) = t / e_num;
		t++;
	}
	t = 0;
	for (size_t i = 3 * e_num; i < (N - n); i++)
	{
		bu(i) = 1 - t / e_num;
		bv(i) = 1;
		t++;
	}

	//for (size_t i = 0; i < b_num; i++)
	//{
	//	cout << bu[i] << " " << bv[i] << endl;
	//}


	//从0，n开始每行取N-n个元素，每列取n个元素

	int test = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < N - n; j++)
		{
			double tmp = lamda(i, j + n);
			if (tmp > 0)
				test++;
			lamdaN_n(i, j) = lamda(i, j + n);
		}
	}



	//cout << "test" << endl;

	b1 = lamdaN_n*bu;
	b2 = lamdaN_n*bv;
	//cout << b1 << endl;
	//cout << b2 << endl;
	//最后求解

	VectorXd resultU(n);
	VectorXd resultV(n);

	resultU = A.householderQr().solve(b1);
	resultV = A.householderQr().solve(b2);

	//cout << resultU << endl;
	//cout << resultV << endl;
	MyMesh::Point               result;
	mesh.request_vertex_texcoords2D();
	MyMesh::TexCoord2D;
	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_texcoord2D(*v_it, OpenMesh::Vec2f(result[0], result[1]));
			i++;
		}
	}

	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_texcoord2D(hv, OpenMesh::Vec2f(result[0], result[1]));

				i++;
			} while (hv != v_it.handle());
			break;
		}

	}

	OpenMesh::IO::Options opt = OpenMesh::IO::Options::VertexTexCoord;
	OpenMesh::IO::write_mesh(mesh, "SPP.obj", opt);

	//内点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);

		if (!is_boundary)
		{
			////pushback ip
			//ip = mesh.point(*v_it);
			//ips.push_back(ip);
			//
			//计算nid属性
			result[0] = resultU[i];
			result[1] = resultV[i];
			result[2] = 0.0;
			//mesh.property(id, *v_it) = i;
			mesh.set_point(*v_it, result);
			i++;
		}
	}
	//边界点
	i = 0;
	for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		auto is_boundary = mesh.is_boundary(*v_it);
		if (is_boundary)
		{
			auto hv = v_it.handle();
			do{
				auto vh = mesh.voh_begin(hv);
				hv = mesh.to_vertex_handle(vh.handle());
				result[0] = bu[i];
				result[1] = bv[i];
				result[2] = 0.0;

				mesh.set_point(hv, result);
				i++;
			} while (hv != v_it.handle());

			break;

		}

	}


	return mesh;
}