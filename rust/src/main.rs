use std::io;

fn main() {
    // 행렬의 크기 입력
    println!("행렬의 행(row)과 열(column)의 크기를 입력하세요 (예: 2 3):");

    let (rows, cols) = {
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let dims: Vec<usize> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().unwrap())
            .collect();
        (dims[0], dims[1])
    };

    // 첫 번째 행렬 입력
    println!("첫 번째 행렬을 입력하세요:");
    let matrix1 = read_matrix(rows, cols);

    // 두 번째 행렬 입력
    println!("두 번째 행렬을 입력하세요:");
    let matrix2 = read_matrix(rows, cols);

    // 두 행렬을 더하기
    let result_matrix = add_matrices(&matrix1, &matrix2, rows, cols);

    // 결과 출력
    println!("두 행렬의 합:");
    print_matrix(&result_matrix, rows, cols);
}

fn read_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = Vec::new();

    for i in 0..rows {
        println!("{}번째 행의 요소를 입력하세요 (예: 1 2 3):", i + 1);
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let row: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().unwrap())
            .collect();
        if row.len() != cols {
            panic!("입력된 열의 개수가 {}와 일치하지 않습니다.", cols);
        }
        matrix.push(row);
    }

    matrix
}

fn add_matrices(
    matrix1: &Vec<Vec<i32>>,
    matrix2: &Vec<Vec<i32>>,
    rows: usize,
    cols: usize,
) -> Vec<Vec<i32>> {
    let mut result = Vec::new();

    for i in 0..rows {
        let mut row = Vec::new();
        for j in 0..cols {
            row.push(matrix1[i][j] + matrix2[i][j]);
        }
        result.push(row);
    }

    result
}

fn print_matrix(matrix: &Vec<Vec<i32>>, rows: usize, cols: usize) {
    for i in 0..rows {
        for j in 0..cols {
            print!("{} ", matrix[i][j]);
        }
        println!();
    }
}

