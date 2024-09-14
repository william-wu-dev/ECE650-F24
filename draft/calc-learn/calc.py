import sys

# YOUR CODE GOES HERE


def main():
    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    while True:
        line = sys.stdin.readline()
        # sample line, do not forget to delete in the final version
        print('READ LINE:', line, file=sys.stdout)
        if line == '':
            break
        # try:
        
        # except Exception e:
        #     print('Error:', str(e), file=sys.stderr)

    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
