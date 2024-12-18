
Screen('Preference', 'SkipSyncTests', 1);
%初期設定
KbName('UnifyKeyNames');
% ウィンドウの固定サイズを指定
% fixedWidth = 1024;
% fixedHeight = 768;

% 利用可能な全てのスクリーンのリストを取得
screens = Screen('Screens');

% サブモニター（セカンダリモニター）のインデックスを選択
% インデックス '0' がメインモニターで、'2' がサブモニター
subScreenNumber = 2;

% もしサブモニターが存在しない場合は、メインモニターを使用
if subScreenNumber > max(screens)
    subScreenNumber = 0;
end

% サブモニターで全画面モードのウィンドウを開く
[window, rect] = Screen('OpenWindow', subScreenNumber, [255 255 255]);



% [fixedWidth, fixedHeight] = Screen('WindowSize', screenNumber);

% % ウィンドウを中央に配置するための座標を計算
% xCenter = (screenWidth - fixedWidth) / 2;
% yCenter = (screenHeight - fixedHeight) / 2;
% winRect = [xCenter yCenter xCenter + fixedWidth yCenter + fixedHeight];

% 固定サイズのウィンドウを開く（全画面モード）
% [window, rect] = Screen('OpenWindow', screenNumber, [255 255 255], winRect);

try
ListenChar(2); % プログラム実行中のキー押し反応がコマンドウィンドウやエディタに入力されないようにする

%ウィンドウの中央の座標を取得
[centerX,centerY] = RectCenter(rect);
% 描画位置を左に寄せるためのオフセット
offsetX = -300;

% ウィンドウを開く
% [window, rect] = Screen('OpenWindow', screenNumber, [255 255 255], winRect);
currentDir = pwd;
movieFiles = {
    fullfile(currentDir, 'Videos', 'L-001.mp4'), % 絶対パスの構築
    fullfile(currentDir, 'Videos', 'L-002.mp4'), % 絶対パスの構築
    fullfile(currentDir, 'Videos', 'R-003.mp4'), % 絶対パスの構築
    fullfile(currentDir, 'Videos', 'L-004.mp4'),
    fullfile(currentDir, 'Videos', 'R-005.mp4'),
    fullfile(currentDir, 'Videos', 'L-006.mp4'),
    fullfile(currentDir, 'Videos', 'L-007.mp4'),
    fullfile(currentDir, 'Videos', 'R-008.mp4'),
    fullfile(currentDir, 'Videos', 'R-009.mp4'),
    fullfile(currentDir, 'Videos', 'R-010.mp4'),
    fullfile(currentDir, 'Videos', 'R-011.mp4'),
    fullfile(currentDir, 'Videos', 'L-012.mp4'),
     };
commands = {
    "L, LOW, 1000", % 1番目の動画ファイル用のコマンド
    "L, LOW, 5000", % 2番目の動画ファイル用のコマンド
    "R, MID, 3000", % 3番目の動画ファイル用のコマンド
     "L, HIGH, 5000",
     "R, HIGH, 500",
     "L, MID, 500",
     "L, LOW, 500",
     "R, MID, 5000",
     "R, LOW, 3000",
     "R, HIGH, 1000",
     "R, MID, 1000",
     "L, HIGH, 3000",
};

responses = []; % 回答を保存するための配列
for iteration = 1:12  % Loop to repeat the process nine times

%movieFilename = 'C:\Users\beeks\OneDrive\ドキュメント\研究室\2023\走行動画\DJI_0007.MP4'; % ここに動画ファイルのパスを指定
 % 配列から動画ファイルのパスを取得
    movieFilename = movieFiles{iteration};
[movie, duration] = Screen('OpenMovie', window, movieFilename);



% 画面に表示
Screen('PlayMovie', movie, 1); % 第2引数の1は正の再生速度を意味する

% 再生開始時刻を記録
movieStartTime = GetSecs;
dataSent = false; % データ送信済みフラグ

while 1
 % 緊急停止のチェック
        [keyIsDown, ~, keyCode] = KbCheck;
        if keyIsDown
            if keyCode(KbName('ESCAPE'))
                % ESCAPEキーが押された場合、プログラムを終了
                Screen('CloseAll');
                ListenChar(0);
                disp('プログラムが緊急停止されました。');
                return;
            end
        end
    tex = Screen('GetMovieImage', window, movie);
    
    if tex <= 0
        break; % 全てのフレームが表示されたらループを終了
    end
    
    Screen('DrawTexture', window, tex);
    Screen('Flip', window);
    Screen('Close', tex);   
% 
% ここでデータ送信のチェック　
    if ~dataSent && (GetSecs - movieStartTime > 15)
        writeline(device, commands{iteration}); 
     disp('データを送信しました。');
        dataSent = true; 
    end
end
  


Screen('PlayMovie', movie, 0); % 動画の再生を停止
Screen('CloseMovie', movie);   % 動画を閉じる




%テキストの設定
Screen('TextSize', window, 24);
Screen('TextFont', window, 'Meiryo');
% 質問のリスト
japanesetextQa = {
    double('振動の強さは適切でしたか？');
    double('振動の長さは適切でしたか？');
    double('振動に嫌悪感を抱きましたか？');
};

japanesetextMa = {
    double('非常に適切である');
    double('非常に適切である');
    double('非常に当てはまる');
    };
japanesetextMi = {
    
    double('全く適切でない');
    double('全く適切でない');
    double('全く当てはまらない')
    };



% 各質問に対してループ 　
for j = 1:length(japanesetextQa)
    % 現在の質問を取得
    currentQuestion = japanesetextQa{j};

    % 画面をクリアして初期状態を描画
    Screen('FillRect', window, [255 255 255]); % 白色で画面を塗りつぶす
    Screen('DrawText', window, currentQuestion, centerX + offsetX, centerY-150, [0,0,0]);   

    % 選択肢と数字の初期描画
    for i = 1:5
        % 選択肢の位置の計算
        choiceX = centerX-100 + offsetX+ (i-1)*150;

        % 1と5の選択肢のみ特別なテキストを使用
        if i == 1
            choiceText = japanesetextMi{j};
        elseif i == 5
            choiceText = japanesetextMa{j};
        else
            choiceText = ''; % 2, 3, 42には特別なテキストは不要
        end

        Screen('DrawText', window, choiceText, choiceX, centerY, [0,0,0]);
        Screen('FrameOval', window, [0, 0, 0], CenterRectOnPointd([0 0 50 50], choiceX + 50, centerY + 100), 2);
        Screen('DrawText', window, num2str(i), choiceX + 50, centerY + 50, [0,0,0]);
    end
    Screen('Flip', window); % 初期状態の画面更新

    % ユーザーの入力を待つ
   waitForKeyPress = true;
while waitForKeyPress
    [keyIsDown, secs, keyCode] = KbCheck;
    if keyIsDown
        if keyCode(KbName('space')) || keyCode(KbName('ESCAPE'))
            % スペースキーまたはエスケープキーが押された場合、プログラムを終了
            Screen('CloseAll');
            return;
        end

        % 1から5までのキー入力を確認
       % 1から5までのキー入力を確認
keyNames = {'1!', '2@', '3#', '4$', '5%'}; % 修正: キー名をシンプルに
keyFound = false;
while waitForKeyPress
    [keyIsDown, secs, keyCode] = KbCheck;
    if keyIsDown
        for k = keyNames
            if keyCode(KbName(k{1}))
                %responses = [responses; {j, k{1}, secs}]; % 不要
                responses = [responses; {j, k{1}(1), secs}];
                waitForKeyPress = false; % 次の質問に進む
                keyFound = true;
                break;
            end
        end

        if keyFound
            WaitSecs(0.5); % 遅延時間の短縮
        else
            % 1から5のキー以外が押された場合は反応しない
            WaitSecs(0.1); % 短い遅延を入れる
                end
            end
        end
     end
    end
end
end

 % 現在の日付と時刻を取得し、フォーマットする
datetimeStr = datestr(now, 'yyyymmdd_HHMMSS');
% 'responses' 配列から Timestamp 列を削除
responses = responses(:, 1:2); % 最初の2列のみを保持21
% 更新された 'responses' 配列をテーブルに変換
T = cell2table(responses, 'VariableNames', {'QuestionID', 'Response'});
% ファイル名に日付と時刻を組み込む
filename = sprintf('responses_%s.csv', datetimeStr);
writetable(T, filename);

 Screen('CloseAll');            % ウィンドウを閉じる
catch
    % エラー時にフルスクリーンのウィンドウを閉じる
    ListenChar(0);
    Screen('CloseAll');            % ウィンドウを閉じる
    psychrethrow(psychlasterror);
    disp(getReport(psychlasterror, 'extended'));
 end

