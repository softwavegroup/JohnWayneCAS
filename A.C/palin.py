def is_palindrome(word):
    # Remove any non-alphanumeric characters and convert to lowercase
    cleaned_word = ''.join(char.lower() for char in word if char.isalnum())
    # Check if the cleaned word is equal to its reverse
    return cleaned_word == cleaned_word[::-1]


# Create a pyramid of words
pyramid = []

# Print the pyramid
for line in pyramid:
    print(line)

def print_number_pyramid(rows):
    for i in range(1, rows + 1):
        # Print spaces
        for j in range(rows - i):
            print(" ", end="")
        # Print numbers
        for j in range(2 * i - 1):
            print(j + 1, end="")
        # Move to the next line after each row
        print()

# Example usage
num_rows = 5
print_number_pyramid(num_rows)
