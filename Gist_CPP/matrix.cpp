#include"matrix.h"

template<class T>
matrix<T>::matrix(){
		row = 0;
		col = 0;
		size = 0;
}

template<class T>
matrix<T>::matrix(int r,int c){
		row = r;
		col = c;
		size = r*c;
}

template<class T>
T matrix<T>::get(int r,int c){
		assert(r<row);
		assert(c<col);
		return realm[r*col+c];
}
template<class T>
void matrix<T>::set(int r,int c,T v){
		assert(r<row);
		assert(c<col);
		realm[r*col+c] = v;
}

template<class T>
void matrix<T>::square(matrix<int> ori){
		assert(ori.row == row);
		assert(ori.col == col);

		for (int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				set(i,j,pow((double)ori.get(i,j),2));

}

template<class T>
void matrix<T>::squre(matrix<double> ori){
		assert(ori.row == row);
		assert(ori.col == col);

		for (int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				set(i,j,pow(ori.get(i,j),2));
}

template<class T>
void matrix<T>::squaresqrt(matrix<int> x,matrix<int> y){
		assert(x.row == row);
		assert(x.col == col);
		assert(y.row == row);
		assert(y.col == col);

		double dtmp;
		for (int i=0;i<row;i++)
			for(int j=0;j<col;j++){
				dtmp = sqrt(pow((double)(x.get(i,j)),2)+pow((double)(y.get(i,j)),2));
				set(i,j,dtmp);
			}

}
template<class T>
void matrix<T>::fftshift(){
		int cshift = col/2;
		int rshift = row/2;
		T tmp;


		if (col%2 == 0){
			for(int  i=0;i<row/2;i++)
				for (int j=0;j<col/2;j++){
					tmp = realm[i*col+j];
					realm[i*col+j] = realm[(i+rshift)*col+j+cshift];
					realm[(i+rshift)*col+j+cshift] = tmp;
				}

				for (int i=row/2;i<row;i++)
					for (int j=0;j<col/2;j++){
						tmp = realm [i*col+j];
						realm[i*col+j] = realm[(i-rshift)*col+j+cshift];
						realm[(i-rshift)*col+j+cshift] =tmp;
					}
		}
		else{
					//int idx;
			/*for (int i=0;i<row;i++){
				tmp = realm[i*col];
				idx = 0;
				int ori = i*col;

				do{
					idx -= shift;
					if (idx<0) idx+=col;
					realm[ori] = realm[i*col+idx];
					ori = i*col+idx;
				}while (idx % col ==0);


				realm[i*col+idx] = tmp;
			}*/
		}
}
template<class T>
void matrix<T>::angle(matrix<int> x,matrix<int> y){
		double a,b;
		double alfa;

		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++){
				a = (double)x.get(i,j);
				b = (double)y.get(i,j);

				if ( a==0 && b==0) set(i,j,0);
				else if ( a==0 && b>0) set(i,j,PI/2);
				else if ( a==0 && b<0) set(i,j,-PI/2);
				else if ( b==0 && a>0) set(i,j,0);
				else if ( b==0 && a<0) set(i,j,PI);
				else{
					alfa = atan(abs(b)/abs(a));
					if ( a>0 && b>0) set(i,j,alfa);
					if ( a>0 && b<0) set(i,j,-alfa);
					if ( a<0 && b>0) set(i,j,PI-alfa);
					if ( a<0 && b<0) set(i,j,-(PI-alfa));
				}
			}
	}
template<class T>
void matrix<T>::add(matrix<double> ori,double d){
		assert(row = ori.row);
		assert(col = ori.col);
		for (int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				set(i,j,ori.get(i,j)+d);
}
template<class T>
void matrix<T>::pi(){
		for (int i=0;i<row;i++)
			for(int j=0;j<col;j++){
				if (get(i,j)<-PI)
					realm[i*col+j]+=2*PI;
				else if (get(i,j)>PI)
					realm[i*col+j]-=2*PI;
			}
		//tr=tr+2*pi*(tr<-pi)-2*pi*(tr>pi);
}

template<class T>
void matrix<T>::doubleprint(){
		for (int i=0;i<1;i++){
			for (int j=0;j<col;j++)
				printf("%f ",get(i,j));
			printf("\n");
		}			
}
template<class T>
void matrix<T>::intprint(){
		for (int i=0;i<row;i++){
			for (int j=0;j<col;j++)
				printf("%d ",get(i,j));
			printf("\n");
		}			
}

template<class T>	
void matrix<T>::squareexps1(matrix<int> fx,matrix<int> fy,double s1){
		double ps = pow(s1,2);
		double dtmp;
		for (int i=0;i<row;i++)
			for (int j=0;j<col;j++){
				dtmp = pow((double)(fx.get(i,j)),2)+pow((double)(fy.get(i,j)),2);
				realm[i*col+j] = exp(-dtmp/ps);
			}
}
