
let currentPlayer = 'X';
let numTurns = 0;
let gameStop = false;
let infoField = document.getElementById('infoField');

window.addEventListener('click', elem => {
    if (gameStop) return;

    let target = elem.target;

    if (target.className === 'cell' && target.innerHTML.trim() === '') {
        target.innerHTML = currentPlayer;

        if (!checkWin()) {
            changePlayer();
        } else {
            infoField.innerHTML = 'Player ' + currentPlayer + ' won!';
            gameStop = true;
            return;
        }

        if (numTurns === 8) {
            infoField.innerHTML = 'It\'s a draw!';
            gameStop = true;
            return;
        }

        numTurns++;
    }
})

function reset() {
    const cells = Array.from(document.querySelectorAll('.cell'));
    cells.forEach(e => e.innerHTML = '');
    currentPlayer = 'O';
    gameStop = false;
    numTurns = 0;
    changePlayer();
}

function checkWin() {
    const cells = Array.from(document.querySelectorAll('.cell')).map(x => x.innerHTML);
    const winConditions = [
        [0, 1, 2],
        [3, 4, 5],
        [6, 7, 8],
        [0, 3, 6],
        [1, 4, 7],
        [2, 5, 8],
        [0, 4, 8],
        [2, 4, 6]
    ];

    for (let win of winConditions) {
        const a = cells[win[0]];
        const b = cells[win[1]];
        const c = cells[win[2]];
        if (a === '' || b === '' || c === '') {
            continue;
        }
        if (a === b && b === c) {
            // Someone won!
            return true;
        }
    }

    return false;
}

function changePlayer() {
    if (currentPlayer === 'X') {
        currentPlayer = 'O';
        infoField.innerHTML = 'Player O\'s turn';
    } else {
        currentPlayer = 'X';
        infoField.innerHTML = 'Player X\'s turn';
    }
}