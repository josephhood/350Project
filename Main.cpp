#pragma once

#include <iostream>
#include <fstream>

#include "Plotter.h"
#include "Matrix.h"
#include "Vector.h"

/*

Template circuit (showing node indices):

					  (2)
			(0)       L1   (1)
		.---------.---UUU---.
		|  ^      |         |   +
	I1 ( ) |   R1 <      R2 <  vout
		|         |         |   -
		'---------+---------'
				 -+-
	[ v0  ]       '
x = [ v1  ]
	[ iL1 ]

vout = v1

Final Circuit:
					  (2)
			(0)       L1          (1)        R3   (3)
		.---------.---UUU---.------------.---VVV---.
		|  ^      |         |  +         |         |+
	I1 ( ) |   R1 <      R2 < vout   C1 ===       ( ) V1  (4)
		|         |         |  -         |         |
		'---------+---------'------------'---------'
				 -+-
	[ v0  ]       '
	[ v1  ]
x = [ iL1 ]
	[ v3  ]
	[ iV1 ]

vout = v1

*/

using namespace std;

int main()
{
	const double tmax = 1.0;
	const double h = 0.001;

	// add new parameters here:
	const double Ra = 0.5;
	const double La = 10.0e-3;
	const double Jm = 0.1;
	const double Bm = 0.005;
	const double Ke = 0.1;
	const double Kt = 0.1;
	const double Va = 10;

	// change the dimensions of the system here:
	Matrix<double> G(7, 7);
	Vector<double> b(7);
	Vector<double> x(7);

	double Wr = 0;
	double Ia = 0;

	// output:
	ofstream fout;
	Plotter plotter("Final Project");
	fout.open("outfile.csv");
	fout << "torque, Wr" << endl;
	plotter.SetLabels("Wr(Angular Frequency)", "ia (A)");

	// stamp G matrix here:

	G.initialize(0);

	G(0, 0) += 1 / Ra;
	G(0, 1) += -1 / Ra;
	G(0, 2) += 0;
	G(0, 3) += 0;
	G(0, 4) += -1;
	G(0, 5) += 0;

	G(1, 0) += -1 / Ra;
	G(1, 1) += 1 / Ra;
	G(1, 2) += 0;
	G(1, 3) += 0;
	G(1, 4) += 1;
	G(1, 5) += 0;

	G(2, 0) += 0;
	G(2, 1) += 0;
	G(2, 2) += 1;
	G(2, 3) += -Ke;
	G(2, 4) += 0;
	G(2, 5) += 0;

	G(3, 0) += 0;
	G(3, 1) += 0;
	G(3, 2) += 0;
	G(3, 3) += Jm / h + 1 / Bm;
	G(3, 4) += -Kt;
	G(3, 5) += 0;

	G(4, 0) += 0;
	G(4, 1) += 1;
	G(4, 2) += 0;
	G(4, 3) += -Ke;
	G(4, 4) += -La / h;
	G(4, 5) += 0;

	G(5, 0) += 0;
	G(5, 1) += 0;
	G(5, 2) += 0;
	G(5, 3) += 0;
	G(5, 4) += -Kt;
	G(5, 5) += 1;

	// Va:
	G(0, 6) += 1;
	G(6, 0) += 1;

	// Bload:
	G(3, 3) += 0.1;
	G(3, 3) += 1.0 / h;

	cout << G;
	for (double t = 0.0; t < tmax; t += h)
	{


		plotter.AddRow(t, 0, x[4]);
		fout << t << "," << x[3] << endl;
		cout << t << "," << x[3] << endl;

		// stamp b vector here:

		b.initialize(0); // clear b vector each time step

		b[0] += 0;
		b[1] += 0;
		b[2] += 0;
		b[3] += x[3] * Jm / h;
		b[4] += -x[4] * La / h;
		b[5] += 0;

		b[6] += Va;
		b[3] += x[3] * 1.0 / h; // mech load

		// solve the system:
		x = G.computeInverse() * b;
	}

	plotter.Plot();
	fout.close();

	return 0;
}
