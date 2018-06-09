# GoogleOauth2Workaround
A hacky way to get users authorized through google Oauth2 (to access Firebase)

1. Set up your firebase project, enable authentication through Google
2. Create a Oauth2.0-Client-ID in developers console and copy Client-ID and Client-Secret into this sketch.
3. Replace "<your_client_id>" in this URL with your client_id and open it in your browser:
    https://accounts.google.com/o/oauth2/auth?redirect_uri=https://localhost&response_type=code&client_id=<your_client_id>&scope=https://www.googleapis.com/auth/firebase.database+https://www.googleapis.com/auth/userinfo.email&approval_prompt=force&access_type=offline
4. Log-in/select with the Google User you would like to authorize.
5. Copy the code after "?code=" from the URL of your browser into this sketch. (without the # at the end)
    This code works only once, if you try to use it in Oauthplayground or somewhere else it wont work here anymore.
    (open URL again and get new code if you used it already)
4. Run the sketch.
5. Save all data printed to Serial, it is the access_token and the refresh_token you will need.
