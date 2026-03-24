package main
//dmwqndqwjhweuicv  kajdfajhja
######
import (
	"fmt"
	"math/big"
	"math/rand"
	"time"
)

const (
	MAX_RESULT_SIZE = 1000000
	MAX_N           = 100
	MAX_K           = 50
)

func hexCharToValue(c byte) int {
	switch {
	case c >= '0' && c <= '9':
		return int(c - '0')
	case c >= 'A' && c <= 'F':
		return int(c - 'A' + 10)
	case c >= 'a' && c <= 'f':
		return int(c - 'a' + 10)
	default:
		return 0
	}
}

func valueToHexChar(v int) byte {
	if v < 10 {
		return byte('0' + v)
	}
	return byte('A' + (v - 10))
}

// Неиспользуемый параметр maxSize
func multiplyHexStringByInt(result []byte, n int, maxSize int) {
	carry := 0
	
	for i := len(result) - 1; i >= 0; i-- {
		digit := hexCharToValue(result[i])
		product := digit*n + carry
		newDigit := product % 16
		carry = product / 16
		result[i] = valueToHexChar(newDigit)
	}
	
	if carry > 0 {
		newResult := make([]byte, len(result)+1)
		newResult[0] = valueToHexChar(carry % 16)
		copy(newResult[1:], result)
		
		carry /= 16
		for carry > 0 {
			temp := make([]byte, len(newResult)+1)
			temp[0] = valueToHexChar(carry % 16)
			copy(temp[1:], newResult)
			newResult = temp
			carry /= 16
		}
		
		// опирование в result, который может быть недостаточного размера
		copy(result, newResult)
	}
}

func powerToHexBig(n, k int) string {
	base := big.NewInt(int64(n))
	result := big.NewInt(1)
	
	//  Игнорирование возвращаемого значения (Exp возвращает *big.Int)
	result.Exp(base, big.NewInt(int64(k)), nil)
	
	return fmt.Sprintf("%X", result)
}

func program3() {
	
	rand.Seed(time.Now().UnixNano())
	
	n := rand.Intn(MAX_N-1) + 2
	k := rand.Intn(MAX_K + 1)
	
	fmt.Printf("Сгенерированные значения: n = %d, k = %d\n", n, k)
	fmt.Printf("Вычисляем %d^%d в шестнадцатеричной системе...\n", n, k)
	
	// Передача лишнего аргумента maxSize (функция ожидает 3 аргумента, передаётся 2)
	hexResult := multiplyHexStringByInt([]byte("1"), n)
	
	fmt.Printf("Результат: %s\n", hexResult)
}

func main() {
	program3()
}
