#include <mpi.h>
#include <iterator>
#include <array>
#include <iostream>

using namespace std;

void first_task(MPI_Group world_group, int rank, int size) {
	if (size >= 9) {
		MPI_Group first_group, second_group, difference_between_groups;
		array<int, 4> first_group_ranks = { 1, 3, 5, 8 };
		array<int, 3> second_group_ranks = { 6, 7, 8 };
		int rank_in_group;
		MPI_Group_incl(world_group, first_group_ranks.size(), first_group_ranks.data(), &first_group);
		MPI_Group_incl(world_group, second_group_ranks.size(), second_group_ranks.data(), &second_group);
		MPI_Group_difference(first_group, second_group, &difference_between_groups);
		MPI_Group_rank(difference_between_groups, &rank_in_group);
		if (rank == 0) {
			cout << "Difference between groups ";
			copy(first_group_ranks.begin(), first_group_ranks.end(), ostream_iterator<int>(cout, " "));
			cout << " and ";
			copy(second_group_ranks.begin(), second_group_ranks.end(), ostream_iterator<int>(cout, " "));
		}
		if (rank_in_group >= 0) {
			cout << endl;
			cout << rank << " -> " << rank_in_group;
		}
		MPI_Group_free(&first_group);
		MPI_Group_free(&second_group);
		MPI_Group_free(&difference_between_groups);
	}
}

int main(int argc, char *argv[]) {
	MPI_Comm dup_comm_world, world_comm; 
	MPI_Group world_group;
	int world_rank, world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_dup(MPI_COMM_WORLD, &dup_comm_world);
	MPI_Comm_group(dup_comm_world, &world_group);
	first_task(world_group, world_rank, world_size);
	MPI_Group_free(&world_group);
	MPI_Comm_free(&dup_comm_world);
	MPI_Finalize();
}