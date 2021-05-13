
function submitted()
{
    let suggestion = document.querySelector('#suggestion').value;
    if (suggestion === '')
    {
        alert('Please enter a suggestion. Your response was blank.');
    }
    else
    {
        alert('Your suggestion, ' + suggestion + ', has been submitted. Thank you for your response!');
    }

    document.querySelector('#message').innerHTML = suggestion + ' has been recorded. You may suggest another chocolate item if you wish.';
}